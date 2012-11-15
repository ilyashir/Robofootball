CONFIG -= warn_off
CONFIG += debug_and_release build_all
CONFIG += qt thread
CONFIG += console

DEBUG_SUFFIX        = 
RELEASE_SUFFIX      = 

win32 {
  DEBUG_SUFFIX      = d
}

CONFIG(debug, debug|release) {
  SUFFIX_STR = $${DEBUG_SUFFIX}
}
else {
  SUFFIX_STR = $${RELEASE_SUFFIX}
}

SHARED_DIR = ../rf-common
DESTDIR = ../rf_bin$${SUFFIX_STR}

unix {
  DEFINES+=UNIX
  DEFINES+=LINUX
  DEFINES+=_GNU_SOURCE

#profiling:
#  CONFIG += debug
#  QMAKE_CFLAGS+=-pg
#  QMAKE_CXXFLAGS+=-pg
#  QMAKE_LFLAGS+=-pg

}

win32 {
  DEFINES += _CRT_SECURE_NO_WARNINGS
  DEFINES += WIN32_LEAN_AND_MEAN
  DEFINES += NOMINMAX

  INCLUDEPATH += ../inc

  QMAKE_CXXFLAGS += /wd4250
  QMAKE_CXXFLAGS += /Fd$(IntDir)

  #enable SSE2
  QMAKE_CXXFLAGS += /arch:SSE2

  #use multithread (non-debug) dll runtime
  #QMAKE_CXXFLAGS_DEBUG += /MD

  #enable debugging in release build
  #QMAKE_CXXFLAGS_RELEASE += /Zi
  #QMAKE_LFLAGS_RELEASE += /DEBUG
}

#protobuf
include( rf-common/proto/proto.pro.inc )


OTHER_FILES +=
