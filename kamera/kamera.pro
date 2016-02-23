TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
LIBS    += -I/usr/local/include/ -L/opt/vc/lib  -lopencv_imgcodecs -lopencv_videoio -lopencv_imgcodecs -lopencv_video -lraspicam -lraspicam_cv -lmmal -lmmal_core -lmmal_util -lopencv_core -lopencv_highgui
SOURCES +=    main.cpp

include(deployment.pri)
qtcAddDeployment()

