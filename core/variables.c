#include "variables.h"
#include <stdlib.h>
#include <string.h>

const uint16_t DEFAULT_PORT = 54832;
const uint8_t IPV4[4] = {127, 0, 0, 1};
uint16_t PORT = DEFAULT_PORT;
const char* DEFAULT_IP = "127.0.0.1";

// allocate memory for IP_ADDRESS that can be modified at runtime
char *IP_ADDRESS = NULL;

void init_ip_address() {
    size_t len = strlen(DEFAULT_IP);
    IP_ADDRESS = (char *)malloc(len + 1); // +1 for null terminator
    if (IP_ADDRESS != NULL) {
        strncpy(IP_ADDRESS, DEFAULT_IP, len);
        IP_ADDRESS[len] = '\0'; // Ensure null termination
    }
}

