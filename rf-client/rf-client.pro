#This is the main project file to build the rf-client

#read the global configuration file
include( ../config.pro.inc )

#where to place built objects
OBJECTS_DIR = ../build/client/obj$${SUFFIX_STR}

#where to place temporary moc sources
MOC_DIR = ../build/client/moc

#where to place auto-generated UI files
UI_DIR = ../build/client/ui

#where to place intermediate resource files
RCC_DIR = ../build/client/resources

#where to place protobuf files
LIB_DIR = ../lib

#where to place MATLAB
MATLAB_DIR = C:/MATLAB/R2009b

unix {
  #add google protocol buffers
  LIBS += -lprotobuf

  #add opengl support
  LIBS += -lGL -lGLU
}

win32 {
  #add libs
  LIBS += $${LIB_DIR}/libprotobuf$${SUFFIX_STR}.lib \
          ws2_32.lib

#  LIBS += $${MATLAB_DIR}/lib/win32/microsoft/libeng.lib \
#          $${MATLAB_DIR}/lib/win32/microsoft/libmat.lib \
#          $${MATLAB_DIR}/lib/win32/microsoft/libmx.lib

  LIBS +=  $${MATLAB_DIR}/extern/lib/win32/microsoft/libeng.lib \
           $${MATLAB_DIR}/extern/lib/win32/microsoft/libmat.lib \
           $${MATLAB_DIR}/extern/lib/win32/microsoft/libmx.lib
}

#enable networking
#QT     += network

#where to build the client executive
TARGET = rf-client

#include actual list of source files for application
include ( rf-client.sources.pro.inc )

DEPENDPATH = INCLUDEPATH

HEADERS +=


