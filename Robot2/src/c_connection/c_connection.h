#ifndef C_CONNECTION_H
#define C_CONNECTION_H

#include "../functions/functions.h"                 // brak
#include "src/Robot2.h"

#include <unistd.h>
#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>
#include <iostream>

#define MAX_buf 256
class C_connection
{

public:
     C_connection(thread_data  *my_data);
    ~C_connection();
    thread_data   *my_data;
    int c_socket;
    sockaddr_in c_from;
    char c_buffer[MAX_buf];
    int recv_size;
    static pthread_mutex_t mutex_buf ;// = PTHREAD_MUTEX_INITIALIZER;
    static pthread_mutex_t mutex_who ;//= PTHREAD_MUTEX_INITIALIZER;
    struct s_pointer *pointer;
    int c_send(int para);
    int c_send(std::string command);
    int c_recv(int para);
    void c_get(int32_t buffor, int i);
    void c_send_recv_RS232 ();
    void c_read_buf (int c_pthread_self);
     
    void c_send_recv_MASTER ();
     int c_analyse();
    bool c_analyse_exit();
    void c_set_buf (int what);
    void c_start_master();
    void c_recv_send_master();
    void l_send_file(std::string path);

    private:
    std::string  str_buf;
};


#endif // C_CONNECTION_H
