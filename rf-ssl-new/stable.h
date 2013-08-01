#pragma once

//Standard Library
#include <iostream>
#include <list>
#include <queue>
#include <algorithm>
#include <stdio.h>
#include <stdint.h>
#include <math.h>

/*
//Windows API
#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
*/

//Utils
#include "bbox.h"
//#include "camera_calibration.h"
#include "colors.h"
#include "conversions.h"
#include "field.h"
#include "framecounter.h"
//#include "framelimiter.h"
//#include "glcamera.h"
#include "geomalgo.h"
#include "geometry.h"
#include "field_filter.h"
#include "gvector.h"
#include "image.h"
#include "image_interface.h"
#include "image_io.h"
#include "lut3d.h"
#include "nkdtree.h"
#include "nvector.h"
#include "pixelloc.h"
#include "pose.h"
#include "qgetopt.h"
#include "quaternion.h"
#include "range.h"
#include "rawimage.h"
#include "ringbuffer.h"
#include "sobel.h"
//#include "texture.h"
//#include "timer.h"
#include "util.h"
#include "vis_util.h"
#include "zoom.h"

//Google Protobuf
#include "messages_robocup_ssl_detection.pb.h"
#include "messages_robocup_ssl_geometry.pb.h"
#include "messages_robocup_ssl_wrapper.pb.h"

//Vartypes
#include "VarNotifier.h"
#include "VarTypes.h"
#include "VarXML.h"
#include "VarTreeView.h"

//Qt Headers
#include <QAction>
#include <QApplication>
#include <QButtonGroup>
#include <QCheckBox>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QFileDialog>
//#include <QGLWidget>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QKeyEvent>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QMessageBox>
#include <QModelIndex>
#include <QMouseEvent>
#include <QMutex>
#include <QObject>
#include <QPainter>
#include <QPushButton>
#include <QSpinBox>
#include <QSplitter>
#include <QStackedWidget>
#include <QString>
#include <QStringList>
#include <QStyleOptionViewItem>
#include <QTime>
#include <QToolBar>
#include <QVariant>
#include <QVBoxLayout>
#include <QWheelEvent>
#include <QWidget>
#include <QWidgetAction>