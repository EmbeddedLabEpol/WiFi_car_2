TEMPLATE = app
CONFIG += console
CONFIG -= qt
QMAKE_CXXFLAGS = -J5 -std=c++14
LIBS +=   -L/usr/local/lib    -lwiringPiDev -lwiringPi  -pthread  -lrt -lpthread -lboost_regex \
   -I/usr/local/include/ -L/opt/vc/lib  -lopencv_imgcodecs -lopencv_videoio -lopencv_imgcodecs -lopencv_video -lraspicam -lraspicam_cv -lmmal -lmmal_core -lmmal_util -lopencv_core -lopencv_highgui -lopencv_objdetect -lopencv_features2d -lopencv_imgproc -lopencv_highgui -lopencv_core

SOURCES += src/parser/parser.cpp   \
 src/c_connection/c_connection.cpp \
 src/functions/functions.cpp \
 src/c_connection/c_connection2.cpp \
   src/logger/logger.cc \
  src/blockQueue/blockqueue.cpp \
    arduPi/arduPi.cpp \
    src/camera/camera.cpp \
    src/Robot2.cpp




HEADERS += src/parser/parser.hpp   \
 src/c_connection/c_connection.h \
 src/functions/functions.h \
    src/logger/logger.hpp \
    src/blockQueue/blockqueue.h \
    src/wiadomosc/wiadomosc.h \
    arduPi/arduPi.h \
    src/camera/camera.hpp \
    src/Robot2.h
