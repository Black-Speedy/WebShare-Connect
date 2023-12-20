#include <openssl/sha.h>
#include <stdio.h>
#include <stdlib.h>


int compute_sha512(const char* path, unsigned char output[SHA512_DIGEST_LENGTH]);