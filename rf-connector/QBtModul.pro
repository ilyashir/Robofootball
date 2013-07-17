
QT       += core  widgets
#QT += gui
QT += network
#Qt += #Qapplication



CONFIG   += console

TARGET = BtModul
TEMPLATE = app


include(src/qextserialport.pri)

SOURCES += main.cpp\
        widget.cpp \
    nxtclass.cpp \
    filework.cpp \
    qsocket.cpp \
    Transfer.cpp \
#    remotecontrol.cpp

HEADERS += widget.h \
    nxtclass.h \
    filework.h \
    qsocket.h \
    Transfer.h \
#    remotecontrol.h

FORMS   +=  widget.ui


win32:{
INCLUDEPATH += win_src
HEADERS += win_src/BtPCModul.h
SOURCES += win_src/BtPCModul.cpp
LIBS+= -L.\ -lBthprops #BTmodul
LIBS+= -L.\ -lOleaut32#BTmodul
HEADERS += win_src/SearchCom.h
LIBS+= -L.\ -lSearchCom #SearchCom
}
