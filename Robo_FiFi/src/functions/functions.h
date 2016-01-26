#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <iostream>
#include <stdlib.h>
#include "../iDom_server/src/iDom_server.h"
#include "../iDom_server/src/c_connection/c_connection.h"

std::string conv_dns (std::string temp);

void ChangeEndianness(int32_t * value, int MAX_MSG_LEN_INT);
void binary(int val);
std::string intToStr(int n);
std::string send_to_arduino (thread_data *my_data_logic, std::string msg);
#endif // FUNCTIONS_H
