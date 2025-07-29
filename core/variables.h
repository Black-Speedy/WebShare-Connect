#ifndef VARIABLES_H
#define VARIABLES_H

#include <stdint.h>

extern const uint16_t DEFAULT_PORT;
extern const uint8_t IPV4[4];
extern const char *DEFAULT_IP;
extern uint16_t PORT; // Current port, can be modified at runtime
extern char *IP_ADDRESS; // Current IP address, can be modified at runtime
void init_ip_address(); // Function to initialize IP_ADDRESS with DEFAULT_IP

#endif // VARIABLES_H
