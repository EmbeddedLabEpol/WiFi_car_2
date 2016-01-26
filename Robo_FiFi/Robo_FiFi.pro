TEMPLATE = app
CONFIG += console
CONFIG -= qt
LIBS +=   -L/usr/local/lib    -lwiringPiDev -lwiringPi -lmpd   -llirc_client -pthread  -lrt -lpthread -lboost_regex

SOURCES += src/iDom_server.cpp src/parser/parser.cpp src/serialib/serialib.cpp \
 src/c_connection/c_connection.cpp \
 src/functions/functions.cpp \
 src/c_connection/c_connection2.cpp \
 src/c_connection/c_connection_node.cpp \
   src/logger/logger.cc \
  src/blockQueue/blockqueue.cpp \




HEADERS += src/parser/parser.hpp src/serialib/serialib.h \
 src/c_connection/c_connection.h \
 src/functions/functions.h \
    src/logger/logger.hpp \
    src/iDom_server.h \
    src/blockQueue/blockqueue.h \
    src/wiadomosc/wiadomosc.h \



