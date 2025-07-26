#ifndef ERRORCODES_H
#define ERRORCODES_H

typedef enum {
    // General errors
    ERR_NO_INPUT           = -0x000001,
    ERR_INVALID_INPUT      = -0x000002,
    ERR_NO_HANDLER         = -0x000003,
    ERR_MEMORY_ALLOCATION  = -0x000004,

    // Port-related errors
    ERR_INVALID_PORT       = -0x000011,
    ERR_PORT_MISSING_VALUE = -0x000012,

    // IP-related errors
    ERR_INVALID_IP         = -0x000111,
    ERR_INVALID_IPV4       = -0x000112,
    ERR_INVALID_IPV6       = -0x000113,
    ERR_IP_MISSING_VALUE   = -0x000114

} ErrorCode;

#endif
