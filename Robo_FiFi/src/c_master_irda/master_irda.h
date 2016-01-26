#ifndef MASTER_IRDA_H
#define MASTER_IRDA_H
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lirc/lirc_client.h>
#include <time.h>
//#include "../wiadomosc/wiadomosc.h"
//#include "../functions/functions.h"
//#include "../logger/logger.hpp"
//#include "../blockQueue/blockqueue.h"
//#include "../c_irda_logic/c_irda_logic.h"
//#include "../iDom_server/src/iDom_server..h"
#include "../c_connection/c_connection.h"
//#include "../iDom_server/src/iDom_server.h"
//extern Logger log_file_mutex;
//extern char * _logfile;

class master_irda
{
public:
    struct lirc_config *config;

    //Timer for our buttons
    int buttonTimer ;
    int buttonMENU;
   //blockQueue char_queue;

    char *code;
    master_irda(thread_data  *my_data);
    void setup();
    void run();
private:
   thread_data  *my_data2;
};

#endif // MASTER_IRDA_H
