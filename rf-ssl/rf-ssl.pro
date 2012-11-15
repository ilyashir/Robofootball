#This is the main project file to build the vision server/gui

#read the global configuration file
include( ../config.pro.inc )

#where to place built objects
OBJECTS_DIR = ../build/rf-ssl/obj$${SUFFIX_STR}

#where to place temporary moc sources
MOC_DIR = ../build/rf-ssl/moc

#where to place auto-generated UI files
UI_DIR = ../build/rf-ssl/ui

#where to place intermediate resource files
RCC_DIR = ../build/rf-ssl/resources

#where to place LIBS
LIB_DIR = ../lib

unix {
  #add libeigen include path
  INCLUDEPATH += /usr/include/eigen2

  #add libdc1394
  LIBS += -ldc1394

  #add google protocol buffers
  LIBS += -lprotobuf

  #add opengl support
  LIBS += -lGL -lGLU
}

win32 {
  #add libs
  LIBS += $$LIB_DIR/libprotobuf$${SUFFIX_STR}.lib \
          $$LIB_DIR/libdc1394$${SUFFIX_STR}.lib \
          ws2_32.lib


  OPENCV_PATH = $$(OPENCV_PATH)

  !isEmpty(OPENCV_PATH){
    message(Compiling with OpenCV in $$(OPENCV_PATH))
        
    INCLUDEPATH += \
      $$OPENCV_PATH/include \
      $$OPENCV_PATH/modules/core/include \
      $$OPENCV_PATH/modules/imgproc/include \
      $$OPENCV_PATH/modules/video/include \
      $$OPENCV_PATH/modules/features2d/include \
      $$OPENCV_PATH/modules/flann/include \
      $$OPENCV_PATH/modules/calib3d/include \
      $$OPENCV_PATH/modules/objdetect/include \
      $$OPENCV_PATH/modules/legacy/include \
      $$OPENCV_PATH/modules/highgui/include \

    #!!! ???? ? lib, ????????? ?????? opencv ??? vs2008
    exists($$OPENCV_PATH\vs2008\lib\Release\opencv_core220.lib) {
      
      debug {
          LIBS += \
            -L$$OPENCV_PATH\vs2008\lib\Debug \
            -lopencv_core220d \
            -lopencv_highgui220d \
            -lopencv_video220d \
            -lopencv_ml220d \
            -lopencv_legacy220d \
            -lopencv_imgproc220d \
            -lopencv_flann220d \
            -lopencv_ffmpeg220d \
            -lopencv_features2d220d \
            -lopencv_contrib220d \
            -L$$OPENCV_PATH\vs2008\3rdparty\lib\Debug \
            -llibjasperd \
            -llibjpegd \
            -llibpngd \
            -llibtiffd \
            -lopencv_lapackd \
            -lzlibd \
            -lvfw32 \
            -lole32 \
      
      }
      release {
          LIBS += \
            -L$$OPENCV_PATH\vs2008\lib\Release \
            -lopencv_core220 \
            -lopencv_highgui220 \
            -lopencv_video220 \
            -lopencv_ml220 \
            -lopencv_legacy220 \
            -lopencv_imgproc220 \
            -lopencv_flann220 \
            -lopencv_ffmpeg220 \
            -lopencv_features2d220 \
            -lopencv_contrib220 \
            -L$$OPENCV_PATH\vs2008\3rdparty\lib\Release \
            -llibjasper \
            -llibjpeg \
            -llibpng \
            -llibtiff \
            -lopencv_lapack \
            -lzlib \
            -lvfw32 \
            -lole32 \
            
        }

    } else {
      message(Unsupported OpenCV version: please use release build of OpenCV 2.2 for MSVC 2008)
    }
  } else {
    message(OPENCV_PATH variable is not set, please set it to a root of OpenCv installation, example: set OPENCV_PATH=C:\OpenCV2.0)
  }
}

#enable gl
QT     += opengl
#enable networking
#QT     += network

#where to build the vision executive
TARGET = rf-vision

#precompiled header
CONFIG += precompile_header
PRECOMPILED_HEADER = stable.h

#include actual list of source files for application
include ( rf-ssl.sources.pro.inc )

DEPENDPATH = INCLUDEPATH

