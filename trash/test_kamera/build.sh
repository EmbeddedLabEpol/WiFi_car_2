#!/bin/sh

g++ kamera.cpp -o  simpletest_raspicam_cv -I/usr/local/include/ -L/opt/vc/lib -lopencv_videoio -lopencv_imgcodecs -lopencv_video -lraspicam -lraspicam_cv -lmmal -lmmal_core -lmmal_util -lopencv_core -lopencv_highgui
