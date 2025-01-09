/**
* @file sender.h
* @brief Header file for the main sender functionality.
*/

#include "common.h"
#include "fileChunks.h"


unsigned char key[crypto_secretbox_KEYBYTES];
unsigned char nonce[crypto_aead_xchacha20poly1305_ietf_NPUBBYTES]; 
unsigned char additional_bytes[crypto_aead_xchacha20poly1305_ietf_ABYTES]; 

/**
 * @brief Sets up a sender socket.
 *
 * Initializes and binds a sender socket to the specified port.
 *
 * @param context The ZeroMQ context.
 * @param port The port to bind the sender socket to.
 * @param threads The number of threads to use.
 * @return A pointer to the created sender socket.
 */
zsock_t* sender(void* context, const char* port, int threads);
/**
 * @brief Sends a file via the sender socket.
 *
 * Reads the specified file, computes its hash, and sends it in chunks via the sender socket.
 *
 * @param serv_sock The sender socket to use for sending the file.
 * @param file_path The path to the file to be sent.
 */
void sender_send(zsock_t* serv_sock, const char* output_file_path);

/**
 * @brief The main function for the sender.
 *
 * Initializes the ZeroMQ context, creates a sender socket, and sends a file via the socket.
 *
 * @param argc The number of command-line arguments.
 * @param argv The array of command-line arguments.
 * @return An integer representing the exit status.
 */
int sender_main(int argc, char const* argv[]);