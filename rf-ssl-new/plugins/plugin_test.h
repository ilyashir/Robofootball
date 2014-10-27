#ifndef TEST_PLUGIN_H
#define TEST_PLUGIN_H
#include <visionplugin.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <string>
#include <map>
#include <sstream>
#include <utility>
#include <fstream>

#include <visionplugin.h>
#include "cmvision_region.h"
#include "messages_robocup_ssl_detection.pb.h"
#include "camera_calibration.h"
#include "field_filter.h"
#include "cmvision_histogram.h"
#include "vis_util.h"
#include "VarNotifier.h"
#include "lut3d.h"

#include "cmpattern_teamdetector.h"
#include "cmpattern_team.h"

#include <QLabel>
#include <QToolButton>

#include <QList>

using namespace cv;
#define pi 3.14159265

class PluginDetectBalls;

class PluginDetectBallsSettings {
friend class PluginDetectBalls;
public:
  VarList * _settings;

  VarInt    * _max_balls;
  VarString * _color_label;
  VarList   * _filter_general;
    VarDouble * _ball_z_height;
    VarInt    * _ball_min_width;
    VarInt    * _ball_max_width;
    VarInt    * _ball_min_height;
    VarInt    * _ball_max_height;
    VarInt    * _ball_min_area;
    VarInt    * _ball_max_area;

  VarList * _filter_gauss;
    VarBool * _ball_gauss_enabled;
    VarInt  * _ball_gauss_min;
    VarInt  * _ball_gauss_max;
    VarDouble * _ball_gauss_stddev;

  VarList   * _filter_too_near_robot;
    VarBool   * _ball_too_near_robot_enabled;
    VarDouble * _ball_too_near_robot_dist;
  VarList   * _filter_histogram;
    VarBool   * _ball_histogram_enabled;
    VarDouble * _ball_histogram_min_greenness;
    VarDouble * _ball_histogram_max_markeryness;
  VarList   * _filter_geometry;
    VarBool   * _ball_on_field_filter;
    VarDouble * _ball_on_field_filter_threshold;
    VarBool   * _ball_in_goal_filter;

  PluginDetectBallsSettings() {

  _settings=new VarList("Ball Detection");

  _settings->addChild(_max_balls = new VarInt("Max Ball Count",1));
  _settings->addChild(_color_label = new VarString("Ball Color","Orange"));

  _settings->addChild(_filter_general = new VarList("Ball Properties"));
    _filter_general->addChild(_ball_z_height = new VarDouble("Ball Z-Height", 30.0));
    _filter_general->addChild(_ball_min_width = new VarInt("Min Width (pixels)", 3));
    _filter_general->addChild(_ball_max_width = new VarInt("Max Width (pixels)", 30));
    _filter_general->addChild(_ball_min_height = new VarInt("Min Height (pixels)", 3));
    _filter_general->addChild(_ball_max_height = new VarInt("Max Height (pixels)", 30));
    _filter_general->addChild(_ball_min_area = new VarInt("Min Area (sq-pixels)", 9));
    _filter_general->addChild(_ball_max_area = new VarInt("Max Area (sq-pixels)", 1000));

  _settings->addChild(_filter_gauss = new VarList("Gaussian Size Filter"));
    _filter_gauss->addChild(_ball_gauss_enabled = new VarBool("Enable Filter",true));
    _filter_gauss->addChild(_ball_gauss_min = new VarInt("Expected Min Area (sq-pixels)", 30));
    _filter_gauss->addChild(_ball_gauss_max = new VarInt("Expected Max Area (sq-pixels)", 40));
    _filter_gauss->addChild(_ball_gauss_stddev = new VarDouble("Expected Area StdDev (sq-pixels)", 10.0));

  _settings->addChild(_filter_too_near_robot = new VarList("Near Robot Filter"));
    _filter_too_near_robot->addChild(_ball_too_near_robot_enabled = new VarBool("Enable Filter",true));
    _filter_too_near_robot->addChild(_ball_too_near_robot_dist = new VarDouble("Distance (mm)",70));

  _settings->addChild(_filter_histogram = new VarList("Histogram Filter"));
    _filter_histogram->addChild(_ball_histogram_enabled = new VarBool("Enable Filter",true));
    _filter_histogram->addChild(_ball_histogram_min_greenness = new VarDouble("Min Greenness",0.5));
    _filter_histogram->addChild(_ball_histogram_max_markeryness = new VarDouble("Max Markeryness",2.0));

  _settings->addChild(_filter_geometry = new VarList("Geometry Filters"));
    _filter_geometry->addChild(_ball_on_field_filter = new VarBool("Ball-In-Field Filter",true));
    _filter_geometry->addChild(_ball_on_field_filter_threshold = new VarDouble("Ball-In-Field Extra Space (mm)",30.0));
    _filter_geometry->addChild(_ball_in_goal_filter = new VarBool("Ball-In-Goal Filter",true));

  }
  VarList * getSettings() {
    return _settings;
  }



};



#define THRESH 10
#define pi 3.14159265

#define MAX_RUNS 100000
#define MAX_REGIONS 1000

#define MIN_AREA 30
#define MAX_AREA 200

#define MAX_COLORS 1000

class Run
{
	public:
		int x,y,width,color,parent,region;
};

class RunList
{
	public:
		Run * runs;
		int used;
                void init()
                {
                   // delete[] runs;
                   // runs=new Run[MAX_REGIONS];
                    used=0;
                }

		RunList(){ runs=new Run[MAX_RUNS];used=0;}
		~RunList(){delete[] runs;}

};

class Region
{
	public:
		int first_run;
		int area;
		double rgb[3];
		double cx,cy,ix,iy,ixy;
        double angle;
		int color;

		
	Region()
	{
		ix=0;
		iy=0;
		ixy=0;
		rgb[0]=0;
		rgb[1]=0;
		rgb[2]=0;
	}
};

class RegionList
{
	public:
		Region * regions;
		int used;
                void init()
                {
                    delete[] regions;
                    regions=new Region[MAX_REGIONS];
                    used=0;
                }
		RegionList()
		{
			regions=new Region[MAX_REGIONS];
			used=0;
		}
		~RegionList()
		{
			delete[] regions;
		}
};

class Hat
{
public:
	Region regs[5];
        int id;
	int color;
	double x,y;
        double phi;
        int used;
        Hat(){x=0;y=0;phi=0;used=0;id=-1;}
        void add(Region reg)
        {
                regs[used++]=reg;
                //printf("used: %d\n",used);
        }
        void computeId();
};

class Color
{
        public:
                uchar bgr[MAX_COLORS][3];
                int used;
                Color(){used=0;}
                void add(uchar * color)
                {
                        bgr[used][0]=color[0];
                        bgr[used][1]=color[1];
                        bgr[used][2]=color[2];
                        ++used;
                }
};

class imageClust
{
	public:


                struct Ball
                {
                    double x;
                    double y;

                };
                void setImg(Mat &img);
                Ball ball;
				
                Hat blues[12];
                Hat yellows[12];

                imageClust();
		RunList rlist;
		RegionList reglist;

		double a,b,phi;
                int thresh;

		//Матрицы для хранения матриц ковариаций и центров цветовых облаков
		Mat covs[5];
		Mat cols[5];
                Color colorset[5];

                uchar calib_colors[4][5];

                Scalar drawColors[5];

		//Изображение
		Mat src;
		Mat gray;

		//void preProcess();
		void ellipse_calc(double ix, double iy, double ixy);
		void rle();
		void clust();
		void regions();
		void recover(Mat &img);
		void colors();
		void readCovs();
		//bool regionCompare (Region i,Region j);
		void invar(Hat &h);
		void findHats();
                void calibrate();

                Mat Wolf(Mat img, int w, double k);

                bool checkColor(int num,uchar * color);



};

class testPlugin : public VisionPlugin
{
    Q_OBJECT


public:
    virtual void mousePressEvent ( QMouseEvent * event, pixelloc loc );
    virtual QWidget * getControlWidget();
    virtual string getName();
    QLabel * info;
    QVBoxLayout * layout;
    imageClust * cl;
    QSpinBox * sb;
    QList<pixelloc>* m_c;

  //-----------------------------
  //local copies of the vartypes tree for better performance
  //these are updated automatically if a change is reported by vartypes
  VarNotifier vnotify;
  bool filter_ball_in_field;
  double filter_ball_on_field_filter_threshold;
  bool filter_ball_in_goal;
  bool filter_ball_histogram;
  double min_greenness;
  double max_markeryness;
  bool filter_gauss;
  int exp_area_min;
  int exp_area_max;
  double exp_area_var;
  double z_height;
  bool near_robot_filter;
  double near_robot_dist_sq;
  int max_balls;

  //Для высчитывания усредненного изображения
  int count;
  Mat avg;

  //-----------------------------

  /////////////////////////////////////////////////////////////////
  VarNotifier _notifier2;
  LUT3D * _lut2;
  VarList * _settings2;

  VarString * _color_label2;
  //TeamDetector *
  int color_id_yellow2;
  int color_id_blue2;
  int color_id_clear2;
  int color_id_ball2;
  int color_id_black2;
  int color_id_field2;


  //CMVision::RegionTree reg_tree2;

  CMPattern::TeamSelector * global_team_selector_blue2;
  CMPattern::TeamSelector * global_team_selector_yellow2;

  CMPattern::TeamDetector * team_detector_blue2;
  CMPattern::TeamDetector * team_detector_yellow2;

  //const CameraParameters& camera_parameters2;
  //const RoboCupField& field2;

  void buildRegionTree2(CMVision::ColorRegionList * colorlist);

  //PluginDetectRobots(FrameBuffer * _buffer, LUT3D * lut, const CameraParameters& camera_params, const RoboCupField& field, CMPattern::TeamSelector * _global_team_selector_blue, CMPattern::TeamSelector * _global_team_selector_yellow);
  ///////////////////////////////////////////////////////

  LUT3D * _lut;
  PluginDetectBallsSettings * _settings;
  bool _have_local_settings;
  int color_id_orange;
  int color_id_pink;
  int color_id_yellow;
  int color_id_field;

  CMVision::Histogram * histogram;

  CMVision::RegionFilter filter;

  const CameraParameters& camera_parameters;
  const RoboCupField& field;

  FieldFilter field_filter;

  bool checkHistogram(const Image<raw8> * image, const CMVision::Region * reg, double min_greenness=0.5, double max_markeryness=2.0);

    //cv::Mat img;
    testPlugin(FrameBuffer * _buffer, LUT3D * lut, const CameraParameters& camera_params, const RoboCupField& field,  CMPattern::TeamSelector * _global_team_selector_blue2, CMPattern::TeamSelector * _global_team_selector_yellow2,  PluginDetectBallsSettings * _settings=0);
    virtual ~testPlugin();
    virtual ProcessResult process(FrameData * data, RenderOptions * options);

public slots:
  void calibClick();
  void readClick();
  void threshChanged(int i);

};

#endif
