#include <stdio.h>
#include "variables.h"
#include "allocatecopystring.h"

const uint16_t DEFAULT_PORT = 54832;
const uint8_t  IPV4[4]      = {127, 0, 0, 1};
uint16_t       PORT         = DEFAULT_PORT;
const char     *DEFAULT_IP  = "127.0.0.1";
char           *FILE_PATH   = NULL; // File path for the application, can be set later

// allocate memory for IP_ADDRESS that can be modified at runtime
char *IP_ADDRESS = NULL;

void init_ip_address() {
    int error = allocate_and_copy_string(&IP_ADDRESS, DEFAULT_IP);
    if (error != 0) {
        fprintf(stderr, "Error: Failed to allocate and copy IP address.\n");
        return;
    }
}