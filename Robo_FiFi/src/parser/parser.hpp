#ifndef PARSER_H_
#define PARSER_H_
#include <string>
#include <vector>

//#include "../logger/logger..hpp"
//#include "../iDom_server/src/iDom_server..h"
#include "../c_master_irda/master_irda.h"
//extern Logger log_file_mutex;
//struct addresses_mac{
//    std::string name_MAC;
//    std::string MAC;
//    bool state;
//    int option1;
//    int option2;
//    int option3;
//    int option4;
//    int option5;
//    int option6;
//};

//struct address_another_servers {
//    int id;
//    std::string SERVER_IP;
//};

//struct config{
//    std::string portRS232;
//    std::string BaudRate;
//    std::string PORT;
//    std::string SERVER_IP;
//    std::string MPD_IP;
//    std::string MOVIES_DB_PATH;
//    int ID_server;
//    std::vector <addresses_mac> A_MAC;
//    int v_delay  ;
//    std::vector <address_another_servers> AAS;

//};



config read_config (const char* file_path  );
#endif
