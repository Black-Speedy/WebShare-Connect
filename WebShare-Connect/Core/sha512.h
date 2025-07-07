#include <openssl/sha.h>
#include <stdio.h>
#include <stdlib.h>

int compute_sha512(const char *path, unsigned char output[SHA512_DIGEST_LENGTH]);
void convert_hash_to_hex_string(unsigned char *hash, char *hex_string, size_t hash_length);