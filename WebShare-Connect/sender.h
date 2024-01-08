/**
* @file sender.h
* @brief Header file for the main sender functionality.
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