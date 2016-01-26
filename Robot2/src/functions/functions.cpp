
#include "functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <strings.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <time.h>
#include <netdb.h>
// przerobka  adresu na ip . //////////////////////////////////
std::string conv_dns (std::string temp){

int i;
struct hostent * he;
struct in_addr ** addr_list;
std::string s_ip;
//const char *buf =temp.c_str();
//scanf( "%99s", buf );

if(( he = gethostbyname( temp.c_str() ) ) == NULL )
{
    herror( "gethostbyname" );
    return "- 1";
}

// print information about this host:
printf( "Official name is: %s\n", he->h_name );

printf( "IP addresses: " );
addr_list =( struct in_addr ** ) he->h_addr_list;
for( i = 0; addr_list[ i ] != NULL; i++ )
{
    printf( "%s ", inet_ntoa( * addr_list[ i ] ) );
    s_ip += inet_ntoa( * addr_list[ i ] );

}
printf( "\n" );

return s_ip;
}


void ChangeEndianness(int32_t * value,int MAX_MSG_LEN_INT)
{
    if (__BYTE_ORDER == __BIG_ENDIAN)
    {
        //printf("big-endian");

    }
    else if( __BYTE_ORDER == __LITTLE_ENDIAN)
    { //cout << "little-endian" << endl
        for (int i =0 ; i < MAX_MSG_LEN_INT ; ++i )
        {

            int32_t result = 0;
            result |= (value[i] & 0x000000FF) << 24;
            result |= (value[i] & 0x0000FF00) << 8;
            result |= (value[i] & 0x00FF0000) >> 8;
            result |= (value[i] & 0xFF000000) >> 24;

            value[i] = result;

        }
    }
}// ChangeEndianness


void binary(int val)
{    for (int i = 31; i >= 0; i--)
    {
        if ((i + 1) % 8 == 0)            std::cout << ' ';        std::cout << ((val >> i) % 2);    }    std::cout << std::endl;
}

std::string intToStr(int n){
     std::string tmp;
     if(n < 0) {
      tmp = "-";
      n = -n;
     }
     if(n > 9)
      tmp += intToStr(n / 10);
     tmp += n % 10 + 48;
     return tmp;
}






std::string send_to_arduino (thread_data *my_data_logic, std::string msg){



    while (go_while)
    {
        usleep(500);
        pthread_mutex_lock(&C_connection::mutex_who);
        if (my_data_logic->pointer.ptr_who[0] == FREE)
        {
            pthread_mutex_lock(&C_connection::mutex_buf);


            my_data_logic->pointer.ptr_who[0]=RS232;
            my_data_logic->pointer.ptr_who[1]= pthread_self();
            buffer=msg;

            pthread_mutex_unlock(&C_connection::mutex_buf);
            pthread_mutex_unlock(&C_connection::mutex_who);
            break;
        }
        pthread_mutex_unlock(&C_connection::mutex_who);

    }

    while (go_while)
    {
        usleep(500);
        pthread_mutex_lock(&C_connection::mutex_who);
        if (my_data_logic->pointer.ptr_who[0] == pthread_self())
        {
            pthread_mutex_lock(&C_connection::mutex_buf);


            my_data_logic->pointer.ptr_who[0]=FREE;
            my_data_logic->pointer.ptr_who[1]= 0;

           msg=buffer;

            pthread_mutex_unlock(&C_connection::mutex_buf);
            pthread_mutex_unlock(&C_connection::mutex_who);
            break;
        }
        pthread_mutex_unlock(&C_connection::mutex_who);

    }

  return msg;
} //end send_to_arduino

