#directories of sources of the vision client

INCLUDEPATH += \
  $${SHARED_DIR}/net \
  $${SHARED_DIR}/proto/cpp \
  $${SHARED_DIR}/util \
  $${SHARED_DIR}/rfprotocol \
  $${MATLAB_DIR}/include \
  $${SHARED_DIR}/vartypes

SOURCES += \
  main.cpp \
  $${SHARED_DIR}/net/netraw.cpp \
  $${SHARED_DIR}/net/robocup_ssl_client.cpp \
    RfClient.cpp \
    Transaction.cpp \
    RfThread.cpp \
    RfData.cpp \
    GraphicsPrimitives.cpp \
    SoccerScene.cpp \
    ClientThreading.cpp

HEADERS += \
  $${SHARED_DIR}/net/netraw.h \
  $${SHARED_DIR}/net/robocup_ssl_client.h \
  $${SHARED_DIR}/net/Pipe.h \
  $${SHARED_DIR}/util/timer.h \
  $${SHARED_DIR}/util/field.h \
  $${SHARED_DIR}/util/field_default_constants.h \
  $${SHARED_DIR}/rfprotocol/rfprotocol.h \
    RfClient.h \
    tmwtypes.h \
    Transaction.h \
    RfThread.h \
    RfData.h \
    GraphicsPrimitives.h \
    SoccerScene.h \
    ClientThreading.h

FORMS += \
    widget.ui

RESOURCES +=
