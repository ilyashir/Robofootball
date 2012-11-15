#directories of sources of the vision client

INCLUDEPATH += \
  $${SHARED_DIR}/net \
  $${SHARED_DIR}/proto/cpp \
  $${SHARED_DIR}/util \
  $${SHARED_DIR}/rfprotocol \
  $${MATLAB_DIR}/include

SOURCES += \
  main.cpp \
  $${SHARED_DIR}/net/netraw.cpp \
  $${SHARED_DIR}/net/robocup_ssl_client.cpp \
    RfClient.cpp

HEADERS += \
  $${SHARED_DIR}/net/netraw.h \
  $${SHARED_DIR}/net/robocup_ssl_client.h \
  $${SHARED_DIR}/net/Pipe.h \
  $${SHARED_DIR}/util/timer.h \
  $${SHARED_DIR}/rfprotocol/rfprotocol.h \
    RfClient.h \
    tmwtypes.h
