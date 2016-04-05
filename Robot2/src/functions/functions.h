#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <iostream>
#include <stdlib.h>
#include "src/Robot2.h"
#include "src/c_connection/c_connection.h"

std::string conv_dns (std::string temp);

void ChangeEndianness(int32_t * value, int MAX_MSG_LEN_INT);
void binary(int val);

std::string send_to_arduino (thread_data *my_data_logic, std::string msg);
std::string  sek_to_uptime(long long secy );
auto map(auto value, auto from_min, auto from_max, auto   to_min, auto to_max);
#endif // FUNCTIONS_H
