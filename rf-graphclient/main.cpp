
#include <QApplication>
#include <QCleanlooksStyle>
#include <QPlastiqueStyle>
#include <QThread>
#include <stdio.h>


#include <windows.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#pragma comment (lib,"QtNetworkd4.lib")

#include <QtCore>

//For Robots Painting
#include "mouse.h"
#include <QtGui>
#include <math.h>
#pragma comment (lib,"QtGuid4.lib")
#pragma comment (lib,"QtCored4.lib")
#pragma comment (lib,"QtXmlPatternsd4.lib")
static const int MouseCount = 12;

//For telling with client
#include "Pipe.h"

//----To watch by somthing------
	float *Blue=(float*)malloc(48*sizeof(float)), *Yellow=(float*)malloc(48*sizeof(float)), *FirstBall=(float*)malloc(4*sizeof(float));

class MyThread : public QThread { 
	public: 
		void run () 
		{ 
			char p[100];
			char color;
			int numb;
			//float N1,N2,N3,N4;
			Pipe_Server MyServer("GraphClient",100);
			MyServer.Connect();
			printf("Start listn Client\n");
			MyServer.listen();
			while(1){
				if(MyServer.Recv(p)){
					//color = 'b';
					//numb='9';
					sscanf(p, "%d %c", &numb, &color);
					//printf("Addres is It: %s\n", p);
					if(color=='b'){
						//printf("OKOKOKOKOK");
						sscanf(p, "%d %c %f %f %f %f", &numb, &color, &Blue[numb], &Blue[numb+12], &Blue[numb+24], &Blue[numb+36]);
						//printf("We have %d %c %f %f %f %f\n", numb, color, Blue[numb], Blue[numb+12], Blue[numb+24], Blue[numb+36]);
					}
					else{
						if(color=='y')
							sscanf(p, "%d %c %f %f %f %f", &numb, &color, &Yellow[numb], &Yellow[numb+12], &Yellow[numb+24], &Yellow[numb+36]);
						else
							sscanf(p, "%d %c %f %f %f", &numb, &color, &FirstBall[0], &FirstBall[1], &FirstBall[2]);
					}					
				}
				else{
					MyServer.Connect();
					printf("Start listn Client again\n");
					MyServer.listen();
				}
			}
		} 
	};

int main(int argc, char **argv)
{
	//----Start----
	QApplication app(argc, argv);

	//--Clear massives--
	for(int i=0; i<48; ++i){
		Blue[i]=0;
		Yellow[i]=0;
	}
	FirstBall[0]=0;
	FirstBall[1]=0;
	FirstBall[2]=0;
	printf("Masives is Cleared\n");

	//---Start telling with client---
	MyThread WorkClient;
	WorkClient.start();
	//MyServer.Recv(p);
	//printf("Addres is It: %s\n", p);
	//sscanf(p, "%p %p %p", &Blue, &Yellow, &FirstBall);
	//printf("Our address is It: %p %p %p\n", Blue, Yellow, FirstBall);
	//printf("Blue [1] is %f", Blue[1]);
	//while(1){}
	//QLocalServer LocalServer;
	//LocalServer.listen("LocalServer");

	//----Create Graphic Window by Qt-----
	printf("Create Graphic Window\n");
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    QGraphicsScene scene;
	/*
	QPushButton *button = new QPushButton("test knopka");
	scene.addWidget(button);
	button->move(20,580);*/
    scene.setSceneRect(0, 0, 800, 600);
    scene.setItemIndexMethod(QGraphicsScene::NoIndex);

    for (int i = 0; i < MouseCount; ++i) {
        Mouse *mouse = new Mouse('y', &Yellow[i], &Yellow[i+12], &Yellow[i+24], &Yellow[i+36]);
        //mouse->setPos(::sin((i * 6.28) / MouseCount) * 200 + 400, ::cos((i * 6.28) / MouseCount) * 200 + 300);
        scene.addItem(mouse);
        mouse->setFlag(QGraphicsItem::ItemIsMovable);
    }
	for (int i = 0; i < MouseCount; ++i) {
        Mouse *mouse = new Mouse('b', &Blue[i], &Blue[i+12], &Blue[i+24], &Blue[i+36]);
        //mouse->setPos(::sin((i * 6.28) / MouseCount) * 200 + 400, ::cos((i * 6.28) / MouseCount) * 200 + 300);
        scene.addItem(mouse);
        mouse->setFlag(QGraphicsItem::ItemIsMovable);
    }
	Mouse *mouse = new Mouse('r', &FirstBall[0], &FirstBall[1], &FirstBall[2], &FirstBall[2]);
    scene.addItem(mouse);
    mouse->setFlag(QGraphicsItem::ItemIsMovable);

    QGraphicsView view(&scene);
    view.setRenderHint(QPainter::Antialiasing);
    view.setBackgroundBrush(QPixmap("field.bmp"));
    view.setCacheMode(QGraphicsView::CacheBackground);
    //view.setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    //view.setDragMode(QGraphicsView::ScrollHandDrag);
    view.setWindowTitle(QT_TRANSLATE_NOOP(QGraphicsView, "SSL Graph Client"));
    //view.centerOn(0,0);//view.baseSize(800,600);
    view.resize(810, 610);
    view.show();

    return app.exec();
}