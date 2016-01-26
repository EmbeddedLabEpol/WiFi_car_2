
#include "c_connection.h"
#include <iostream>

void C_connection::c_write_buf(char * buf){

    for (unsigned int i =0 ; i< sizeof(c_buffer);++i)
    {
        c_buffer[i]=' ';
    }
    for (unsigned int i =0 ; i< strlen( buf);++i)
    {

        c_buffer[i]=buf[i];
    }
    c_buffer[strlen(buf)]='\0';

}


void C_connection::c_read_buf(int c_pthread_self)
{



}

void C_connection::l_send_log(std::string path){

    std::fstream log_file;
    log_file.open(path.c_str(),std::ios::in);
    if( log_file.good() == false )
    {
        c_write_buf(" can not open file !");
        c_send(0);
        c_recv(0);
        return;
    }


    else
    {
        std::cout << "otwarty plik" << std::endl;
        while( !log_file.eof() )
        {

            for (int i =0 ; i< MAX_buf-1 ; ++i)
            {
                if (log_file.eof()==1)
                {

                    break;
                }
                c_buffer [i] = log_file.get();

            }
            c_buffer[MAX_buf-1]='\0';
             std::cout << " wielkosc bufora z send "<< strlen(c_buffer) << std::endl;
            c_send(0);
            c_recv(0);

        }



    log_file.close();
}
}






