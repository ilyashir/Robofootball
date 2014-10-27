#include "plugin_test.h"
#include <stdio.h>
#include <exception>
#include <qmath.h>
#include <sstream>


testPlugin::testPlugin( FrameBuffer * _buffer, LUT3D * lut, const CameraParameters& camera_params, const RoboCupField& field ,  CMPattern::TeamSelector * _global_team_selector_blue2, CMPattern::TeamSelector * _global_team_selector_yellow2,PluginDetectBallsSettings * settings)
    : VisionPlugin ( _buffer ), camera_parameters ( camera_params ), field ( field )
{
    _settings=settings;
    //_have_local_settings=false;
    if ( _settings==0 ) {
      _settings = new PluginDetectBallsSettings();
      //_have_local_settings=true;
    }
    //printf("testPlugin inti\n");
    cl=new imageClust();
    cl->readCovs();
    m_c = new QList<pixelloc>();
    avg = Mat::zeros(480,640,CV_32FC3);
    count=0;
}


testPlugin::~testPlugin()
{
   // delete cl;
}

void testPlugin::mousePressEvent(QMouseEvent *event, pixelloc loc)
{
    //info->setText(QString::number(loc.x)+QString(" ")+QString::number(loc.y));
    m_c->push_back(loc);
}

ProcessResult testPlugin::process(FrameData * data, RenderOptions * options)
{
    (void)options;

    ImageInterface * source = &(data->video);

    RawImage * processedImg;
    if ((processedImg=(RawImage *)data->map.get("qwerty")) == 0)
    {
        processedImg=(RawImage *)data->map.insert("qwerty",new RawImage());
    }

    processedImg -> allocate(COLOR_RGB8,source -> getWidth(),source -> getHeight());
    processedImg -> setWidth(source -> getWidth());
    processedImg -> setHeight(source -> getHeight());

    Mat img=Mat::zeros(source -> getHeight(),source -> getWidth(),CV_8UC3);
   // Mat img(source -> getHeight(),source -> getWidth(),CV_8UC3);
    uchar* mat = img.data;

    int          source_size    = source->getNumPixels();
    rgb *        source_pointer = (rgb*)(source->getData());

    for (int i=0;i<480;++i)
        for (int j=0;j<640;++j)
        {
            mat[3*(i*640+j)+2]=source_pointer[i*source->getWidth()+j].r;
            mat[3*(i*640+j)+1]=source_pointer[i*source->getWidth()+j].g;
            mat[3*(i*640+j)+0]=source_pointer[i*source->getWidth()+j].b;
        }
//    if (count<9)
//    {
//        ++count;
//        Mat tmp;
//        img.convertTo(tmp,CV_32FC3);
//        avg+=tmp/9;

//        return ProcessingOk;
//    }
//    if (count==9)
//    {
//        avg.convertTo(avg,CV_8UC3);
//        ++count;
//    }

//   // Mat img1; img.copyTo(img1);
//    subtract(img,avg,img);
    cl->setImg(img);

    cl->findHats();
    cl->recover(img);
    printf("after recover\n");
   // Mat out;
    //cv::normalize(avg,avg,0,255,NORM_MINMAX,-1,Mat());
//    avg.convertTo(avg,CV_8UC3);
//    cvtColor(avg,avg,CV_BGR2RGB);
    //normalize(avg,)
   // info->setPixmap(QPixmap::fromImage(QImage((const unsigned char*)(avg.data), avg.cols, avg.rows, QImage::Format_RGB888)));
    printf("after draw\n");
    //info->setPixmap();
    info->setPixmap(QPixmap::fromImage(QImage((const unsigned char*)(cl->gray.data), cl->gray.cols, cl->gray.rows, QImage::Format_Indexed8)));

    {
        // From primordial ssl
        SSL_DetectionFrame * detection_frame = 0;

        detection_frame= ( SSL_DetectionFrame * ) data->map.get ( "ssl_detection_frame" );
        if ( detection_frame == 0 )
            detection_frame= ( SSL_DetectionFrame * ) data->map.insert ( "ssl_detection_frame",new SSL_DetectionFrame() );

        detection_frame->clear_balls();

        SSL_DetectionBall* ball = detection_frame->add_balls();

        // Do not know what "confidence" is
        ball->set_confidence ( 1 );


        // From pixel 2D to 3D
        z_height= _settings->_ball_z_height->getDouble();
        vector2d pixel_pos ( cl->ball.x,cl->ball.y );
        vector3d field_pos_3d;
        camera_parameters.image2field ( field_pos_3d,pixel_pos,z_height );
        vector2d field_pos ( field_pos_3d.x,field_pos_3d.y );

        // ball->set_area ( it->reg->area ); // this is optional
        ball->set_x ( field_pos_3d.x );
        ball->set_y ( field_pos_3d.y );
        ball->set_pixel_x ( cl->ball.x );
        ball->set_pixel_y ( cl->ball.y );
        printf("after ball\n");

        ::google::protobuf::RepeatedPtrField< ::SSL_DetectionRobot >* robotlist_blue=0;
        ::google::protobuf::RepeatedPtrField< ::SSL_DetectionRobot >* robotlist_yellow=0;
        robotlist_blue=detection_frame->mutable_robots_blue();
        robotlist_yellow=detection_frame->mutable_robots_yellow();

        int count=0;
        for(int i=0; i<12; i++){
            if (cl->blues[i].id>-1)
            {
                SSL_DetectionRobot * robot = 0;
                if (robotlist_blue->size() < count+1)
                {
                    robotlist_blue->Add();
                    robot = robotlist_blue->Mutable(count);
                }
                else
                {
                    robot = robotlist_blue->Mutable(count);
                    robot->Clear();
                }
                ++count;
                //  printf("%d size: %d\n",i,robotlist_blue->size());

                vector2d reg_img_center( cl->blues[i].x , cl->blues[i].y );
                vector3d reg_center3d;
                camera_parameters.image2field(reg_center3d,reg_img_center, 140.0 );

                robot->set_confidence(count );
                robot->set_robot_id( i );
                robot->set_orientation(cl->blues[i].phi);
                robot->set_pixel_x( cl->blues[i].x );
                robot->set_pixel_y( cl->blues[i].y );
                robot->set_x( reg_center3d.x );
                robot->set_y( reg_center3d.y );
                robot->set_height( 140.0 );
                printf("Blues %d coords: %f %f %f\n",robot->robot_id(),robot->x(),robot->y(),robot->orientation());
            }
        }
        // printf("after b34es\n");
        count=0;
        for(int i=0; i<12; i++)
        {
            if (cl->yellows[i].id>-1)
            {
                SSL_DetectionRobot * robot = 0;
                if (robotlist_yellow->size() < count+1)
                {
                    robotlist_yellow->Add();
                    robot = robotlist_yellow->Mutable(count);
                }
                else
                {
                    robot = robotlist_yellow->Mutable(count);
                    robot->Clear();
                }
                ++count;
                vector2d reg_img_center( cl->yellows[i].x , cl->yellows[i].y );
                vector3d reg_center3d;
                camera_parameters.image2field(reg_center3d,reg_img_center, 140.0 );

                robot->set_confidence( count );
                robot->set_robot_id( i );
                robot->set_orientation(cl->yellows[i].phi);
                robot->set_pixel_x( cl->yellows[i].x );
                robot->set_pixel_y( cl->yellows[i].y );
                robot->set_x( reg_center3d.x );
                robot->set_y( reg_center3d.y );
                robot->set_height( 140.0 );
                printf("Yellows %d coords: %f %f %f\n",robot->robot_id(),robot->x(),robot->y(),robot->orientation());
            }
        }
    }



    //uchar * mat1 = img.data;

    for (int i=0;i<m_c->size();++i)
    {
        circle(img,Point(m_c->at(i).x,m_c->at(i).y),25,Scalar(255));
    }

//    Rect rect(0,0,100,100);
//    img(rect) = Mat::zeros(100,100,CV_8UC3);

    for (int i=0;i<480;++i)
        for (int j=0;j<640;++j)
        {
            source_pointer[i*source->getWidth()+j].r= mat[3*(i*640+j)+2];
            source_pointer[i*source->getWidth()+j].g= mat[3*(i*640+j)+1];
            source_pointer[i*source->getWidth()+j].b= mat[3*(i*640+j)+0];
        }
   // img1.copyTo(cl->src);
    printf("count: %d\n",count);
    printf("after conversion\n");

    return ProcessingOk;
}

Mat imageClust::Wolf(Mat img, int w, double k){

    printf("start wolf\n");



    Scalar S = sum(img);
    double Smax=S[0]>S[1] ? S[0]:S[1];
    Smax=S[2]>Smax ? S[2]:Smax;

    uchar *mat=img.data;
    int cols = img.cols;
    int rows = img.rows;

    for (int i=0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
        {
            mat[3*(i*cols + j)+0] *= 2-S[0]/Smax;
            mat[3*(i*cols + j)+1] *= 2-S[1]/Smax;
            mat[3*(i*cols + j)+2] *= 2-S[2]/Smax;
        }


        cvtColor(img,img,CV_BGR2GRAY);

        double minI, maxI;
        minMaxLoc(img, &minI, &maxI);

        img=(img-minI)*255/(maxI-minI);

        img.convertTo(img,CV_32F);

        Mat m, s;
        Mat kernel = Mat::ones(w,w,CV_32F) / (float)(w*w);

        filter2D(img, m, CV_32F, kernel);

        Mat t = Mat(img.rows,img.cols,CV_32F);
        t = img - m;
        filter2D(t.mul(t), s, CV_32F, kernel);
        sqrt(s,s);


        double minS, maxS;
        minMaxLoc(s, &minS, &maxS);

        t = m+k*(s/maxS-1).mul(m-minI);

        Mat binimg = (img>t & s>15);

        Mat se =(Mat_<unsigned char>(4,4) << 0, 1, 1, 0,
                                             1, 1, 1, 1,
                                             1, 1, 1, 1,
                                             0, 1, 1, 0);
        morphologyEx(binimg, binimg, MORPH_OPEN, se);
        return binimg;
}


void testPlugin::calibClick(){

    lock();

    cl->calibrate();
    unlock();

    info->setText("Lal!");
    //layout->update();

}

void testPlugin::readClick()
{
    cl->readCovs();
    info->setText("Looks like read :)");
}

void imageClust::setImg(Mat &img)
{
    rlist.init();
    reglist.init();
    //Mat tmp1, tmp2;
   // src.convertTo(tmp1,CV_32FC3);
    //img.convertTo(tmp2,CV_32FC3);
    //((Mat)((tmp1+tmp2)/2)).convertTo(src,CV_8UC3);
    //src=(src+img)/2;
    //addWeighted(src,0.5,img,0.5,0.0,src);
    src = img;
//    gray = Wolf(img,15,-0.1);

    gray=Mat(img.size(),CV_8UC1);
    cvtColor(img,gray,CV_BGR2GRAY);
    blur(gray,gray,Size(3,3));
    threshold(gray,gray,thresh,255,CV_THRESH_BINARY);
    //readCovs();

}

void testPlugin::threshChanged(int i)
{
    cl->thresh=i;
    info->setText(QString::number(cl->thresh));
}

QWidget* testPlugin::getControlWidget()
{
    sb=new QSpinBox();
    sb->setMaximum(255);
    sb->setFixedWidth(40);
    sb->setValue(10);

    QWidget * widget=new QWidget();
    layout=new QVBoxLayout();
    QToolButton * calib=new QToolButton();
    QToolButton * read=new QToolButton();
    read->setText("Read covs");

    calib->setText("Push me!");
    connect(calib,SIGNAL(clicked(bool)),this,SLOT(calibClick()));
    connect(read,SIGNAL(clicked(bool)),this,SLOT(readClick()));
    connect(sb,SIGNAL(valueChanged(int)),this,SLOT(threshChanged(int)));

    info=new QLabel();
    info->setText(QString::number(sb->value()));
    layout->addWidget(info);
    layout->addWidget(calib);
    layout->addWidget(read);
    layout->addWidget(sb);
    widget->setLayout(layout);
    return widget;
}

string testPlugin::getName()
{
    return "Test";
}


imageClust::imageClust()
{
   // src = Mat::zeros(480,640,CV_8UC3);
    //Mat::zeros()

    drawColors[0]=Scalar(255,0,0);
    drawColors[1]=Scalar(0,255,255);
    drawColors[2]=Scalar(0,255,0);
    drawColors[3]=Scalar(255,0,255);
    drawColors[4]=Scalar(0,0,255);

    calib_colors[0][0]=0;
    calib_colors[0][1]=3;
    calib_colors[0][2]=3;
    calib_colors[0][3]=3;
    calib_colors[0][4]=2;

    calib_colors[1][0]=0;
    calib_colors[1][1]=2;
    calib_colors[1][2]=3;
    calib_colors[1][3]=2;
    calib_colors[1][4]=3;

    calib_colors[2][0]=1;
    calib_colors[2][1]=3;
    calib_colors[2][2]=3;
    calib_colors[2][3]=3;
    calib_colors[2][4]=3;

    calib_colors[3][0]=1;
    calib_colors[3][1]=2;
    calib_colors[3][2]=2;
    calib_colors[3][3]=2;
    calib_colors[3][4]=2;
    thresh=10;

        //readCovs();
}

void imageClust::ellipse_calc(double ix, double iy, double ixy)
{
    if ((ixy == 0) && (ix == iy))
    {
        b = sqrt(ix);
        a = sqrt(iy);
        phi = 0;
    }
    double D = sqrt((ix-iy)*(ix-iy) + 4*ixy*ixy);
    b = sqrt((ix+iy - D)/2);
    a = sqrt((ix+iy + D)/2);

    double d = (ix-iy)/D;
    if (d > 0)
        phi = atan(sqrt((1-d)/(1+d)));
    else
        phi = pi/2 - atan(sqrt((1+d)/(1-d)));
    if (ixy < 0)
        phi = -phi;
}

//rle-кодировка бинаризованного изображения
void imageClust::rle()
{
	Run* runs=rlist.runs;
	uchar *mat=gray.data;

	int width=src.cols;
        int rows=src.rows;
	Run r;
	int x,tmp,col;
	int j=0;

        for (int i=0;i<rows;++i)
	{
		r.y=i;
		x=0;
		while(x<width)
		{
                        col=mat[i*width+x];

			tmp=x;
                        while(x<width && mat[i*width+x]==col) ++x;
				
			r.x=tmp;
			r.width=x-tmp;
			r.parent=j;
			r.color=col;
			runs[j++]=r;
		}
	}
	rlist.used=j;
    printf("used: %d\n",rlist.used);
}

//Выделение связных областей из rle-кодировки
void imageClust::clust()
{
	//Указатель на массив рядов
	Run * runs=rlist.runs;
	//Два текущих ряда
	Run r1,r2;
	//Количество рядов всего
	int used=rlist.used;
	//Счетчики для первой и второй строки, соответственно
	int l1=0;
	int l2=1;
	//Всякая вспомогательная фигня
	int i,j,s;
	//Счетчик второй строки должен начинаться с первого ряда второй строки :)
	while (runs[l2].y==0) ++l2;
	//Инициализируем ряды первой и второй строк
	r1=runs[l1];
	r2=runs[l2];
	//Непонятное что-то
	s=l2;
	//Пока счетчик второй строки не станет больше количества рядов
	while (l2<used)
	{
		//Если у нас два ряда одного цвета и не черные
		if (r1.color==r2.color && r1.color>0)
		{
			//Условие пересечения двух рядров
			if ((r1.x-(r2.x+r2.width))*(r2.x-(r1.x+r1.width))>=0)
			{
				//Что-то снова непонятное
				if (s!=l2)
				{
					runs[l2].parent = r2.parent = r1.parent;
					s = l2;
				}
				//Если родители не совпадают
				else if (r1.parent!=r2.parent)
				{
					//Находим корневых родителей обоих рядов
					i = r1.parent;
					while(i != runs[i].parent) i = runs[i].parent;
					j = r2.parent;
					while(j != runs[j].parent) j = runs[j].parent;
					//Выбираем родителя с наименьшим значением
					//(нужно, чтобы сохранить свойство полноты
					//ориентированного ациклического графа)
					//ну и переназначаем родителей
					if (i<j)
					{
						runs[j].parent=i;
						runs[l1].parent = runs[l2].parent = r1.parent = r2.parent = i;
					}
					else
					{
						runs[i].parent=j;
						runs[l1].parent = runs[l2].parent = r1.parent = r2.parent = j;
					}
				}
			}
		}

		//Переход к следующему ряду
		i = (r1.x + r1.width) - (r2.x + r2.width);
		if(i >= 0) 
		{
			r2 = runs[++l2];
		}
		if(i <= 0) 
		{
			r1 = runs[++l1];
		}
	}

	//Восстанавливаем оставшиеся родительские элементы
	for(i=0; i<used; i++)
	{
		j = runs[i].parent;
		runs[i].parent = runs[j].parent;
	}
    //printf("rle: %d\n",used);
}

//Выделение регионов
//Регион - кластер, подходящий по характеристикам
//+Запиленный в отдельный класс
//+Формирование списка регионов
void imageClust::regions()
{
	uchar * mat=src.data;
	int cols=src.cols;

	Run curr;
	Region * regions=reglist.regions;
	Run * runs=rlist.runs;
	int used=rlist.used;
	int n=0;
	int y,parent,width;
	
	for (int i=0;i<used;++i)
	{
		if (runs[i].color>0)
		{

			curr=runs[i];
			width=curr.width;
			
			if (curr.parent==i)
			{
               // printf("if %d\n",curr.parent);
				runs[i].region=n;
				
				regions[n].first_run=i;
				regions[n].area=width;
				regions[n].cx=width*(2*curr.x+width-1)/2;
				regions[n].cy=curr.y*width;
				++n;
				//printf("n: %d\n",n);
			}
			
			else
			{
               // printf("else parent:%d\n",curr.parent);
				parent=runs[curr.parent].region;
				runs[i].region=parent;
				//parent=runs[curr.parent].parent;
				//runs[i].parent=parent;
				regions[parent].area+=width;
				regions[parent].cx+=width*(2*curr.x+width-1)/2;
				regions[parent].cy+=curr.y*width;
			}
		}
	}
	reglist.used=n;
    printf("regions1: %d\n",n);
	
	Region * r=new Region[100];
	int count=0;
	for (int i=0;i<n;++i)
	{
		regions[i].cx/=regions[i].area;
		regions[i].cy/=regions[i].area;
		if (regions[i].area>MIN_AREA && regions[i].area<MAX_AREA)
		{
			r[count]=regions[i];
			++count;
		}
	}
        delete[] regions;
	reglist.regions=r;
	reglist.used=count;
	regions=reglist.regions;
	
    printf("regions2: %d\n",n);
	n=reglist.used;
	Region * final_r=new Region[n];
	
	count =0;
	for (int i=0;i<n;++i)
	{
		curr=runs[regions[i].first_run];
		parent=curr.parent;
		for (int j=0;j<used;++j)
		{
			if (runs[j].parent==parent)
			{
				y=runs[j].y;
				for (int k=runs[j].x;k<runs[j].x+runs[j].width;++k)
				{
					regions[i].ix+=(k-regions[i].cx)*(k-regions[i].cx);
					regions[i].iy+=(y-regions[i].cy)*(y-regions[i].cy);
					regions[i].ixy+=(k-regions[i].cx)*(y-regions[i].cy);

					regions[i].rgb[0]+=mat[3*(y*cols+k)];
					regions[i].rgb[1]+=mat[3*(y*cols+k)+1];
					regions[i].rgb[2]+=mat[3*(y*cols+k)+2];

				}
				//regions[i].color[0]+=mat[runs[j].y*cols+runs[j]
	
			}
		}
		regions[i].ix/=regions[i].area;
		regions[i].iy/=regions[i].area;
		regions[i].ixy/=regions[i].area;

		regions[i].rgb[0]/=regions[i].area;
		regions[i].rgb[1]/=regions[i].area;
		regions[i].rgb[2]/=regions[i].area;

		ellipse_calc(regions[i].ix,regions[i].iy,regions[i].ixy);
		stringstream ss;
		if (a/b<1.5 && a*b<140)
		{
			final_r[count]=regions[i];
			/*
			ss<<regions[i].first_run;
			putText(src,ss.str(),Point(regions[i].cx,regions[i].cy),CV_FONT_HERSHEY_PLAIN,1,Scalar(0,255,0));
			ss.str("");
			*/
			++count;
		}
	}
        delete[] r;
	reglist.regions=final_r;
	reglist.used=count;
    printf("regions: %d\n",reglist.used);
	

}

//Функция для отображения найденных областей
void imageClust::recover(Mat &src)
{
     printf("recover0\n");
	uchar * mat=src.data;
	Run curr;
	Run * r=rlist.runs;
	Region * reg=reglist.regions;

	int cols=src.cols;
	int parent;
	int n=reglist.used;
	int run_n=rlist.used;
	int y;
	

//        for (int i=0;i<n;++i)
//	{
//		curr=r[reg[i].first_run];
//		parent=curr.parent;
//		for (int j=0;j<run_n;++j)
//		{
//			if (r[j].parent==parent)
//			{
//				y=r[j].y;
//				for (int k=r[j].x;k<r[j].x+r[j].width;++k)
//                                {
//                                    mat[3*(y*cols+k)]=255;
//                                     mat[3*(y*cols+k)+1]=255;
//                                      mat[3*(y*cols+k)+2]=255;
//                                    //src.at<Scalar>(k,y)=drawColors[r[j].color];
//                                    //src.at<Scalar>(k,y)=Scalar(255);
//                                }
//			}
//		}
//    }

    printf("recover\n");
    for (int i=0;i<n;++i)
    {
        circle(src,Point(reg[i].cx,reg[i].cy),5,drawColors[reg[i].color]);
    }
}

//Определение цветов областей по готовым матрицам ковариаций
void imageClust::colors()
{

	Run * runs=rlist.runs;
	Region * regs=reglist.regions;
	Region curr_reg;
	int n=reglist.used;
	int res_col;
	double min=6;
	double curr;
	Mat cov(3,3,CV_64F);
	Mat col(1,3,CV_64F);
	Mat res;
	stringstream ss;
	
	for (int j=0;j<n;++j)
	{
		curr_reg=regs[j];
		min=6;
		for (int i=0;i<5;++i)
		{
			ss<<i<<".txt";
            ifstream in(ss.str().c_str());
			ss.str("");
			col.at<double>(0,0)=cols[i].at<double>(0,0)-curr_reg.rgb[0];
			col.at<double>(0,1)=cols[i].at<double>(0,1)-curr_reg.rgb[1];
			col.at<double>(0,2)=cols[i].at<double>(0,2)-curr_reg.rgb[2];
			res=col*covs[i]*col.t();
			curr=res.at<double>(0,0);
			if (curr<min)
			{
				min=curr;
				res_col=i;
			}
		}
		if (min<6)
		{
			regs[j].color=res_col;
		}
        else
            regs[j].color=5;
      //  printf("color: %d\n",regs[j].color);
	}
}

//Предварительное считывание матриц ковариаций из файлов.
//Nuff said.
void imageClust::readCovs()
{
	stringstream ss;
	for (int i=0;i<5;++i)
	{
		covs[i]=Mat(3,3,CV_64F);
		cols[i]=Mat(1,3,CV_64F);
		ss<<i<<".txt";
        ifstream in(ss.str().c_str());
		ss.str("");

		in>>cols[i].at<double>(0,0)>>cols[i].at<double>(0,1)>>cols[i].at<double>(0,2);
               // printf("%d center: %f %f %f\n",i,cols[i].at<double>(0,0),cols[i].at<double>(0,1),cols[i].at<double>(0,2));
               // printf("%d cov:\n",i);

		for (int k=0;k<3;++k)
        {
              //  printf("\t");
            for (int j=0;j<3;++j)
            {
                in>>covs[i].at<double>(k,j);
                // printf("%f ",covs[i].at<double>(k,j));
            }
           // printf("\n");
        }
	}
}

bool regionCompare (Region i,Region j) { return (i.angle<j.angle); }

void imageClust::invar(Hat &h)
{
   // printf("inver starting\n");
	Region regs[5];
	
	for (int i=0;i<5;++i)
	{
		regs[i]=h.regs[i];
	}

	double sumx=0;
	double sumy=0;
	for (int i=0;i<5;++i)
	{
		sumx+=regs[i].cx;
		sumy+=regs[i].cy;
	}
	sumx/=5;
	sumy/=5;

	double dist=100;
	double curr_dist;
	int cnt_no=0;
	Point cnt(regs[0].cx,regs[0].cy);
	
	for (int i=0;i<5;++i)
	{
		curr_dist=sqrt((sumx-regs[i].cx)*(sumx-regs[i].cx)+(sumy-regs[i].cy)*(sumy-regs[i].cy));
		
		if (curr_dist<dist)
		{
			
			dist=curr_dist;
			cnt_no=i;
			cnt=Point(regs[i].cx,regs[i].cy);		
		}
	}

    //double angle;

	h.regs[0]=regs[cnt_no];

	for (int i=0;i<4;++i)
	{
		if (i>=cnt_no)
			regs[i]=regs[i+1];

		regs[i].cx-=h.regs[0].cx;
		regs[i].cy-=h.regs[0].cy;
		regs[i].angle=atan2(regs[i].cy,regs[i].cx);
	}
	sort(regs,regs+4,regionCompare);
	for (int i=0;i<4;++i)
	{
		regs[i].cx+=h.regs[0].cx;
		regs[i].cy+=h.regs[0].cy;
	}

	int fr1=0;
	int fr2=0;
	int j1,j2;
	dist=0;
	for (int i=0;i<4;++i)
	{
		j1 = i+1>3 ? 0 : i+1;
		j2 = i-1<0 ? 3 : i-1;
		curr_dist=sqrt((double)(regs[i].cx-regs[j1].cx)*(regs[i].cx-regs[j1].cx)+(regs[i].cy-regs[j1].cy)*(regs[i].cy-regs[j1].cy));
		if (curr_dist>dist)
		{
			dist=curr_dist;
			fr1=i;
			fr2=j1;
		}
		curr_dist=sqrt((double)(regs[i].cx-regs[j2].cx)*(regs[i].cx-regs[j2].cx)+(regs[i].cy-regs[j2].cy)*(regs[i].cy-regs[j2].cy));
		if (curr_dist>dist)
		{
			dist=curr_dist;
			fr1=i;
			fr2=j2;
		}
	}
		
	int tmp=0;
	double a,b,c1;
	
	a=regs[fr2].cy-regs[fr1].cy;
	b=regs[fr1].cx-regs[fr2].cx;
	c1=regs[fr1].cy*(regs[fr2].cx-regs[fr1].cx)-regs[fr1].cx*(regs[fr2].cy-regs[fr1].cy);
	
	double val=a*cnt.x+b*cnt.y+c1;
	if (val>0)
	{
		tmp=fr1;
		fr1=fr2;
		fr2=tmp;
	}
	
	//h.regs[1]=regs[fr1];
	//h.regs[2]=regs[fr2];
	//printf("%d fr2: %d %d\n",h.regs[1].first_run,fr1,fr2);
	if (fr2>fr1 && abs(fr2-fr1)==1 || (fr2<fr1) && abs(fr2-fr1)>1)
	{
		for (int i=1;i<5;++i)
		{
			
			if (i-fr1<1)
				tmp=4+i-fr1;
			else
				tmp=i-fr1;
			h.regs[tmp]=regs[i-1];
		}
	}
	
	else
	{
		for (int i=1;i<5;++i)
		{
			if (i-fr1-1<=0)
				tmp=1+abs(i-fr1-1);
			else
				tmp=4-i+fr1;
			h.regs[tmp]=regs[i-1];
		}
	}
    //printf("inver worked\n");
}

void Hat::computeId()
{

    id=(regs[1].color-2)+2*(regs[2].color-2)+4*(regs[3].color-2)+8*(regs[4].color-2);
    id=id>11? 11:id;

}

void imageClust::findHats()
{
    //lock();
	rle();
    //printf("rle worked!\n");
    clust();
   // printf("clust worked!\n");
	regions();
   // printf("regions worked!\n");
	colors();
   // printf("colors worked!\n");
    for (int i=0;i<12;++i)
    {
        blues[i].id=-1;
        yellows[i].id=-1;

    }



	int n=reglist.used;
	Region * regs=reglist.regions;
    //Run * runs=rlist.runs;
	double dist;
	int count=0;
    //int blues_count=0;
   // int yellows_count=0;
    double cx=0;
    double cy=0;
    int thr=21;
	for (int i=0;i<n;++i)
    {

        if (regs[i].color<2)
        {
           // printf("got it!\n");
            Hat h;
            count=0;
            h.regs[count++]=regs[i];
            //circle(src,Point(h.regs[0].cx,h.regs[0].cy),1,Scalar(255,255,255),2);
            circle(src,Point(h.regs[0].cx,h.regs[0].cy),thr,drawColors[regs[i].color],1);
            cx=0;
            cy=0;
            for (int j=0;j<n;++j)
            {
                dist=sqrt((regs[i].cx-regs[j].cx)*(regs[i].cx-regs[j].cx)+(regs[i].cy-regs[j].cy)*(regs[i].cy-regs[j].cy));
               // printf("%d-%d color: %d dist: %f\n",i,j,regs[j].color,dist);

                if (dist<thr && (regs[j].color==2 || regs[j].color==3) && i!=j && count<5)
                {
                   // printf("%d %d %d\n",count++,i,j);
                    h.regs[count++]=regs[j];
                    cx+=regs[j].cx;
                    cy+=regs[j].cy;
                  //  printf("added %d\n",j);
                }
            }

            cx/=4;
            cy/=4;
            circle(src,Point(cx,cy),1,drawColors[regs[i].color],1);

           // printf("after dist\n");

            if (count==5)
            {

                invar(h);
                h.x=h.regs[0].cx;
                h.y=h.regs[0].cy;
                h.color=h.regs[0].color;

                double dy=(h.regs[3].cy-h.regs[4].cy - h.regs[1].cy+h.regs[2].cy)/2;
                double dx=-(h.regs[3].cx-h.regs[4].cx - h.regs[1].cx+h.regs[2].cx)/2;
                h.phi=atan2(dy,dx);
               // printf("phi: %f\n",h.phi);
                h.computeId();
                printf("%d color id: %d\n",h.color,h.id);
                if (h.color==0)
                    blues[h.id]=h;
                else
                    yellows[h.id]=h;
                //hats[hats_count++]=h;

                line(src,Point(h.regs[1].cx,h.regs[1].cy),Point(h.regs[2].cx,h.regs[2].cy),Scalar(0,regs[i].color*255,255),2);
                circle(src,Point(h.regs[2].cx,h.regs[2].cy),5,Scalar(0,0,255),2);
                line(src,Point(h.regs[3].cx,h.regs[3].cy),Point(h.regs[4].cx,h.regs[4].cy),Scalar(0,regs[i].color*255,255),2);
            }


       }
        if (regs[i].color==4)
        {
            ball.x=regs[i].cx;
            ball.y=regs[i].cy;
        }
    }
    //imwrite("img.png",src);
    printf("findhats worked!\n");
     printf("yo!\n");

  // unlock();
}


void imageClust::calibrate()
{
    //imwrite("out.png",src);
//        rle();
//        printf("calib rle worked!\n");
//        clust();
//        printf("calib clust worked!\n");
//        regions();
//        printf("calib regions worked!\n");

    //printf("-*-*-*-*-*-*calib worked lol!-*-*-*-*-*-*\n");
       // system("pause");


        // --------------------------------- //
        int src_cols=src.cols;
        uchar * mat=src.data;

        Region * regs=reglist.regions;
        Run * runs=rlist.runs;

        int n=reglist.used;
        printf("calib regs: %d\n",n);
        double cx=0;
        double cy=0;
        double dist=200;
        double curr_dist;
        int ball;
        stringstream ss;

        for (int i=0;i<n;++i)
        {
                cx+=regs[i].cx;
                cy+=regs[i].cy;
        }

        cx/=n;
        cy/=n;
        printf("%f %f\n",cx,cy);
        //circle(src,Point(cx,cy),3,Scalar(0,255,0),-1);

        for (int i=0;i<n;++i)
        {
                curr_dist=sqrt((regs[i].cx-cx)*(regs[i].cx-cx)+(regs[i].cy-cy)*(regs[i].cy-cy));
                if (curr_dist<dist)
                {
                        dist=curr_dist;
                        ball=i;
                }
        }

        cx=regs[ball].cx;
        cy=regs[ball].cy;
        printf("%f %f\n",cx,cy);
//        //circle(src,Point(regs[ball].cx,regs[ball].cy),5,Scalar(0,0,255),-1);
        Hat hats[4];
        for (int i=0;i<n;++i)
        {
                if (i==ball) continue;
                if (regs[i].cx-cx<0)
                        if (regs[i].cy-cy<0)
                                hats[0].add(regs[i]);
                        else
                                hats[3].add(regs[i]);
                else
                        if (regs[i].cy-cy<0)
                                hats[1].add(regs[i]);
                        else
                        {
                            printf("2 %d\n",i);
                                hats[2].add(regs[i]);
                        }
        }

        int parent;
        for (int i=0;i<4;++i)
        {printf("h %d %d\n",i,hats[i].used);

                invar(hats[i]);
                for (int j=0;j<5;++j)
                {
//                    printf("i: %d j: %d\n",i,j);
                        parent=runs[hats[i].regs[j].first_run].parent;
                        for (int k=0;k<rlist.used;++k)
                        {
//                            printf("%d %d\n",rlist.used,k);
                                if (runs[k].parent==parent)
                                {
//                                    printf("%d %d %d \n",rlist.used,i,j);
                                        for (int s=runs[k].x;s<runs[k].x+runs[k].width;++s)
                                        {

                                                if (checkColor(calib_colors[i][j],mat+3*(runs[k].y*src_cols+s)))
                                                        colorset[calib_colors[i][j]].add(mat+3*(runs[k].y*src_cols+s));
                                        }
                                }
                        }
                }
        }
      //  system("pause");
        printf("after colorcheck\n");

        parent=runs[regs[ball].first_run].parent;
        for (int k=0;k<rlist.used;++k)
        {
            if (runs[k].parent==parent)
            {
                for (int s=runs[k].x;s<runs[k].x+runs[k].width;++s)
                {
                    if (checkColor(4,mat+3*(runs[k].y*src_cols+s)))
                        colorset[4].add(mat+3*(runs[k].y*src_cols+s));
                }
            }
        }

        int k=0;
        for (int i=0;i<colorset[k].used;++i)
            printf("%d blue color: %d %d %d\n",i,colorset[k].bgr[i][0],colorset[k].bgr[i][1],colorset[k].bgr[i][2]);



        for (int j=0;j<5;++j)
        {
                ss<<j<<".txt";
                ofstream out(ss.str().c_str());

                cols[j]=Mat(1,3,CV_64F);
                covs[j]=Mat(3,3,CV_64F);
                double bgr[3]={0,0,0};
                Mat cov,mu;
                Mat colors(colorset[j].used,3,CV_8U);
                uchar * color_ptr=colors.data;
                int color_size=colorset[j].used;
                for (int i=0;i<color_size;++i)
                {
                    color_ptr[3*i]=colorset[j].bgr[i][0];
                    color_ptr[3*i+1]=colorset[j].bgr[i][1];
                    color_ptr[3*i+2]=colorset[j].bgr[i][2];

                    bgr[0]+=colorset[j].bgr[i][0];
                    bgr[1]+=colorset[j].bgr[i][1];
                    bgr[2]+=colorset[j].bgr[i][2];
                }
                cols[j].at<double>(0,0)=bgr[0]/=color_size;
                cols[j].at<double>(0,1)=bgr[1]/=color_size;
                cols[j].at<double>(0,2)=bgr[2]/=color_size;

                out<<cols[j].at<double>(0,0)<<" "<<cols[j].at<double>(0,1)<<" "<<cols[j].at<double>(0,2)<<"\n";

                calcCovarMatrix(colors,cov,mu,CV_COVAR_NORMAL|CV_COVAR_ROWS|CV_COVAR_SCALE);
                cov.at<double>(0,0)+=0.01;
                cov.at<double>(1,1)+=0.01;
                cov.at<double>(2,2)+=0.01;

                covs[j]=cov.inv();
                for (int i=0;i<3;++i)
                {
                    for (int k=0;k<3;++k)
                        out<<covs[j].at<double>(i,k)<<" ";
                    out<<"\n";
                }

                printf("%d covar: %f %f %f\n",j,covs[j].at<double>(0,0),covs[j].at<double>(0,1),covs[j].at<double>(0,2));
                printf("%d covar: %f %f %f\n",j,covs[j].at<double>(1,0),covs[j].at<double>(1,1),covs[j].at<double>(1,2));
                printf("%d covar: %f %f %f\n",j,covs[j].at<double>(2,0),covs[j].at<double>(2,1),covs[j].at<double>(2,2));
                printf("--------\n");

                ss.str("");

        }



}

bool imageClust::checkColor(int num,uchar * color)
{

 //   bool hack=0;
 //   printf("check %d %d\n",num,colorset[num].used);
    for (int i=0;i<colorset[num].used;++i)
    {
//        printf("%d checkcolro!\n",i);
        if (colorset[num].bgr[i][0]==color[0] && colorset[num].bgr[i][1]==color[1] && colorset[num].bgr[i][2]==color[2])
        {
            return false;
        }
    }

        return true;
}
