#ifndef VARIABLES_H
#define VARIABLES_H

#include <stdint.h>

extern const uint16_t DEFAULT_PORT;
extern const uint8_t  IPV4[4];
extern const char     *DEFAULT_IP;
extern uint16_t       PORT;
extern char           *IP_ADDRESS;
extern char           *FILE_PATH;
void init_ip_address();

#endif // VARIABLES_H