/**
 * @file receiver.h
 * @brief Header file for the main receiver functionality.
 */

#include "common.h"
#include "fileChunks.h"

/**
 * @brief Sets up a receiver socket and connects to the sender.
 *
 * Initializes and connects a receiver socket to the specified sender port.
 *
 * @param context The ZeroMQ context.
 * @param port The port of the sender to connect to.
 * @param threads The number of threads to use.
 * @return A pointer to the created receiver socket.
 */
zsock_t* receiver(void* context, const char* ip_address, const char* port, int threads);

/**
 * @brief Receives a file via the receiver socket.
 *
 * Receives the file header, then receives file chunks and writes them to the specified output file path.
 * Computes the hash of the received file and compares it with the expected hash.
 *
 * @param receiver_sock The receiver socket used for receiving the file.
 * @param output_file_path The path to save the received file.
 */
void receiver_receive(zsock_t* receiver_sock, const char* output_file_path);

/**
 * @brief The main function for the receiver.
 *
 * Initializes the ZeroMQ context, creates a receiver socket, connects to the sender, and receives a file via the socket.
 * Compares the received file's hash with the expected hash for integrity verification.
 *
 * @param argc The number of command-line arguments.
 * @param argv The array of command-line arguments.
 * @return An integer representing the exit status.
 */
int receiver_main(int argc, char *argv[]);