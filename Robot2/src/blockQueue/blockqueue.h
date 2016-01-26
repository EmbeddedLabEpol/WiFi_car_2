#ifndef BLOCKQUEUE_H
#define BLOCKQUEUE_H

#include <stdlib.h>
#include <queue>
#include <pthread.h>
#include <iostream>
//#include <wiringPi.h>

#include "../logger/logger.hpp"
//extern Logger log_file_mutex;
//extern char * _logfile;
class blockQueue
{
public:
    blockQueue();
     static pthread_mutex_t mutex_queue_char ;

static std::queue < char > _charQ;

void _add(char X);
char _get();
int  _size();

};

#endif // BLOCQUEUE_H
