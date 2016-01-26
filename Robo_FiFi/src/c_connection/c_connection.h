#ifndef C_CONNECTION_H
#define C_CONNECTION_H

#include "../functions/functions.h"                 // brak
//#include "../logger/logger.hpp"
//#include "../iDom_server/src/iDom_server..h"'
#include "../functions/mpd_cli.h"
#include "../iDom_server/src/iDom_server.h"

#include <unistd.h>
#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>


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
     int c_send(char command[]);
     int c_recv(int para);
 int32_t c_return(int iterator);
    void c_get(int32_t buffor, int i);
    void c_send_recv_RS232 ();
    void c_read_buf (int c_pthread_self);
    void c_write_buf(char *buf);
    void c_send_recv_MASTER ();
     int c_analyse();
    bool c_analyse_exit();
    void c_set_buf (int what);


    // metody dla noda
     void c_start_master();
    void c_recv_send_master();

     // wysylanie loga
    void l_send_log(string path);

    private:
    std::string temporary_str;

};


#endif // C_CONNECTION_H
