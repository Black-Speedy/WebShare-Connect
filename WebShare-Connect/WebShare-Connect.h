
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <czmq.h>
#include <sodium.h>
#include "common.h"
//if on windows
#ifdef _WIN32
	#include <OpenSSL/ssl.h>
//if on linux
#endif
#ifdef __linux__
	#include <openssl/ssl.h>
#endif