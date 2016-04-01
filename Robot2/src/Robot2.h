#ifndef GLOBAL_H
#define GLOBAL_H
#include <iostream>
#include <fstream>
#include <strings.h>
#include <string>
#include <cstdlib>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <vector>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <time.h>
#include <boost/lexical_cast.hpp>
#include <wiringPi.h>

//#include <lirc/lirc_client.h>


// MOJE BIBLIOTEKI
#include "logger/logger.hpp"
#include "../arduPi/arduPi.h"
#include "blockQueue/blockqueue.h"
#include "camera/camera.hpp"
constexpr int  MAX_MSG_LEN =18;
constexpr int MAX_CONNECTION =10;
constexpr int FREE =1;
constexpr int RS232 =11;
constexpr int ok   =  0;

#define log_file_cout  f_log //std::cout   zmien f_log na std::cout  i bedzie wypisywac na ekran
#define log_file_mutex f_log

constexpr int ON= 1;
constexpr int OFF =0;



extern std::string  _logfile  ;

extern Logger log_file_mutex;

extern std::string buffer;

//extern int max_msg  ;

extern bool go_while  ;


struct s_pointer{
    unsigned int *ptr_who;
    int32_t *ptr_buf;
};

struct Thread_array_struc {
    pthread_t  thread_ID;
    std::string thread_name;
    int thread_socket;
};
struct robot_config {
    bool face_detection;
    bool send_auto_STOP;
    int button16_counter =0 ;
};

struct config{
    std::string portRS232;
    std::string BaudRate;
    std::string PORT;
    std::string SERVER_IP;
    unsigned int ID_server;
    int v_delay  ;
};
struct thread_data{
    int s_client_sock;
    sockaddr_in from;
    config *server_settings;
    s_pointer pointer;
    robot_config robot1;
    Thread_array_struc *main_THREAD_arr;
    time_t start;
    time_t now_time;

};


struct thread_data_rs232{

    std::string portRS232;
    std::string BaudRate;
    struct s_pointer pointer;

};


#endif // GLOBAL_H
