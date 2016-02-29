TEMPLATE = app
CONFIG += console
CONFIG -= qt
LIBS +=   -L/usr/local/lib    -lwiringPiDev -lwiringPi  -pthread  -lrt -lpthread -lboost_regex

SOURCES += src/iDom_server.cpp src/parser/parser.cpp   \
 src/c_connection/c_connection.cpp \
 src/functions/functions.cpp \
 src/c_connection/c_connection2.cpp \
   src/logger/logger.cc \
  src/blockQueue/blockqueue.cpp \
    arduPi/arduPi.cpp




HEADERS += src/parser/parser.hpp   \
 src/c_connection/c_connection.h \
 src/functions/functions.h \
    src/logger/logger.hpp \
    src/iDom_server.h \
    src/blockQueue/blockqueue.h \
    src/wiadomosc/wiadomosc.h \
    arduPi/arduPi.h
