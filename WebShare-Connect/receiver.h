/**
* @file receiver.h
* @brief Header file for the main receiver functionality.
*/

/**
 * @brief Chunk sizes in bytes for various file sizes.
 */
#define CHUNK_SIZE_1MB    (1 << 12)                 /**< Chunk size for   1 MB:   4 KiB */
#define CHUNK_SIZE_100MB  (1 << 16)                 /**< Chunk size for 100 MB:  64 KiB */
#define CHUNK_SIZE_1GB    (1 << 18)                 /**< Chunk size for   1 GB: 256 KiB */
#define CHUNK_SIZE_10GB   (1 << 20)                 /**< Chunk size for  10 GB:   1 MiB */
#define CHUNK_SIZE_20GB   (1 << 21)                 /**< Chunk size for  20 GB:   2 MiB */
#define CHUNK_SIZE_35GB   ((1 << 21) + (1 << 20))   /**< Chunk size for  35 GB:   3 MiB */
#define CHUNK_SIZE_50GB   (1 << 22)                 /**< Chunk size for  50 GB:   4 MiB */

 /**
  * @brief File sizes in bytes for different capacities.
  */
#define FILE_SIZE_1MB		        1 * 1000 * 1000LL	/**<   1 MB in bytes */
#define FILE_SIZE_100MB		      100 * 1000 * 1000LL	/**< 100 MB in bytes */
#define FILE_SIZE_1GB		 1 * 1000 * 1000 * 1000LL	/**<   1 GB in bytes */
#define FILE_SIZE_10GB		10 * 1000 * 1000 * 1000LL	/**<  10 GB in bytes */
#define FILE_SIZE_20GB		20 * 1000 * 1000 * 1000LL	/**<  20 GB in bytes */
#define FILE_SIZE_35GB		35 * 1000 * 1000 * 1000LL	/**<  35 GB in bytes */
#define FILE_SIZE_50GB		50 * 1000 * 1000 * 1000LL	/**<  50 GB in bytes */

#include "sender.h"

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
zsock_t* receiver(void* context, const char* port, int threads);

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
int receiver_main(int argc, char const* argv[]);