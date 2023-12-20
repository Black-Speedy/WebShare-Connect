#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>


/*
 * add macros and function declarations which should be
 * common between the name server and peer programs in this file.
 */

#define COMMAND_REGISTER        1
#define COMMAND_RETREIVE        2
#define COMMAND_INFORM          3

#define STATUS_OK               1
#define STATUS_PEER_EXISTS      2
#define STATUS_BAD_REQUEST      3
#define STATUS_OTHER            4
#define STATUS_MALFORMED        5

#define IP_LEN                  16
#define PORT_LEN                8

#define REQUEST_HEADER_LEN      28
#define REPLY_HEADER_LEN        80 

#define MAX_MSG_LEN             8196

#define PATH_LEN                128

 /* slightly less awkward string equality check */
#define string_equal(x, y) (strncmp((x), (y), strlen(y)) == 0)
