#include <openssl/sha.h>
#include <stdio.h>
#include <stdlib.h>

int compute_sha512(const char*path, unsigned char output[SHA512_DIGEST_LENGTH]) {
    // Open file
    FILE*file = fopen(path, "rb");
    if (!file) {
        perror("File opening failed");
        return -1;
    }

    // Initialize SHA512 context
    SHA512_CTX sha512;
    SHA512_Init(&sha512);

    // Read the File
    const int bufSize = 32768;
    char      *buffer = malloc(bufSize);
    if (!buffer) {
        fclose(file);
        return -1;
    }

    // Update SHA512 context
    int bytesRead;
    while ((bytesRead = fread(buffer, 1, bufSize, file))) {
        SHA512_Update(&sha512, buffer, bytesRead);
    }
    rewind(file); // Reset file pointer (not sure if this is necessary)
    free(buffer);
    fclose(file);

    // Finalize SHA512 context
    SHA512_Final(output, &sha512);

    return 0;
}

void convert_hash_to_hex_string(unsigned char*hash, char*hex_string, size_t hash_length) {
    for (size_t i = 0; i < hash_length; i++) {
        sprintf(hex_string + (i * 2), "%02x", hash[i]);
    }
    hex_string[hash_length * 2] = '\0'; // Null-terminate the string
}