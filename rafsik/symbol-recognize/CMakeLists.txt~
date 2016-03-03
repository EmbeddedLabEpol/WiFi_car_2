PROJECT(RobotDemo)

cmake_minimum_required(VERSION 2.8)

FIND_PACKAGE( OpenCV REQUIRED )
#FIND_PACKAGE(raspicam REQUIRED)

ADD_EXECUTABLE( demo main.cpp symbol_finder.cpp )
TARGET_LINK_LIBRARIES( demo ${OpenCV_LIBS} pthread)
#TARGET_LINK_LIBRARIES( demo ${OpenCV_LIBS} ${raspicam_CV_LIBS} pthread)
