
#include "c_connection.h"
#include <iostream>



void C_connection::c_read_buf(int c_pthread_self)
{



}
void C_connection::l_send_jpg (std::string path){


    std::fstream _file;
    _file.open(path.c_str(),std::ios::in);
    str_buf = "";
    if( _file.good() == false )
    {
        str_buf =" can not open file !";
        c_send(0);
        c_recv(0);
        return;
    }


    else
    {
        //std::cout << "otwarty plik" << std::endl;
        str_buf.erase();
        //std::cout << "wyczyszczone" << std::endl;
        while( !_file.eof() )
        {

            for (int i =0 ; i< 64000-1 ; ++i)
            {
                if (_file.eof()==1)
                {
                    //str_buf.erase(str_buf.length()-1, str_buf.length());
                    //std::cout << "koniec " << std::endl;
                    break;
                }
                str_buf += _file.get();

            }
            //c_buffer[MAX_buf-1]='\0';
           // std::cout << " wielkosc bufora z send "<< str_buf.length() << std::endl;
            c_recv(0);
           // std::cout << " odebralem " << c_buffer << std::endl;
            c_send(0);

            str_buf.erase();

        }

        str_buf = "END#";
        c_recv(0);
        c_send(0);

std::cout << "close" << std::endl;

        _file.close();
    }
}

void C_connection::l_send_file(std::string path)
{

    std::fstream log_file;
    log_file.open(path.c_str(),std::ios::in);
    if( log_file.good() == false )
    {
        str_buf ="cannot open the file log ";
        c_send(0);
        c_recv(0);
        return;
    }


    else
    {
        std::cout << "otwarty plik" << std::endl;
        str_buf.erase();
        while( !log_file.eof() )
        {

            for (int i =0 ; i< MAX_buf-1 ; ++i)
            {
                if (log_file.eof()==1)
                {
                    str_buf.erase(str_buf.length()-1, str_buf.length());
                    break;
                }
                str_buf += log_file.get();

            }
            //c_buffer[MAX_buf-1]='\0';
            std::cout << " wielkosc bufora z send "<< str_buf.length() << std::endl;
            c_send(0);
            c_recv(0);
            str_buf.erase();

        }

        log_file.close();
    }
}






