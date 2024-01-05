#include "WebShare-Connect.h"
#include "client.h"
#include "sha512.h"

/**
 * @brief Struct representing file information.
 */
typedef struct {
    // This is the header for the file
    int64_t file_size;
    int chunk_size;
    unsigned char hash[SHA512_DIGEST_LENGTH];
} file_header_t;

/**
 * @brief Sets up a client socket and connects to the server.
 *
 * Initializes and connects a client socket to the specified server port.
 *
 * @param context The ZeroMQ context.
 * @param port The port of the server to connect to.
 * @param threads The number of threads to use.
 * @return A pointer to the created client socket.
 */
zsock_t* client(void* context, const char *port, int threads)
{
	int io_threads = threads;
	zmq_ctx_set(context, ZMQ_IO_THREADS, io_threads);
	assert(zmq_ctx_get(context, ZMQ_IO_THREADS) == io_threads);

	zsock_t* client_sock = zsock_new(ZMQ_PAIR);
	assert(client_sock);

	int rc = zsock_connect(client_sock, "tcp://2.tcp.eu.ngrok.io:%s", port); // change to to server ip when running on different machines
	assert(rc != -1);

	zsock_set_rcvtimeo(client_sock, 2000); // 2s timeout for recv

	return client_sock;
}

/**
 * @brief Receives a file via the client socket.
 *
 * Receives the file header, then receives file chunks and writes them to the specified output file path.
 * Computes the hash of the received file and compares it with the expected hash.
 *
 * @param client_sock The client socket used for receiving the file.
 * @param output_file_path The path to save the received file.
 */
void client_receive(zsock_t* client_sock, const char* output_file_path) {
    printf("\n-- File Reception --\n");

    file_header_t header;
    size_t header_size = sizeof(file_header_t);
    byte* header_buf = NULL;

    // Receive the header
    int rc = zsock_recv(client_sock, "b", &header_buf, &header_size);
    if (rc == -1) {
        fprintf(stderr, "Error: Failed to receive header.\n");
        return;
    }

    // Validate the header size
    if (header_size != sizeof(file_header_t)) {
        fprintf(stderr, "Error: Received header size does not match expected size.\n");
        free(header_buf);
        return;
    }

    // Copy the received data into the header structure
    if (header_buf == NULL) {
		fprintf(stderr, "Received header is NULL\n");
		return;
	}
    memcpy(&header, header_buf, sizeof(header));
    free(header_buf); // Free the received buffer after copying

    int64_t file_size = header.file_size;
    int chunk_size = header.chunk_size;
    printf("Header received successfully.\n");
    printf("Expected file size: %lld bytes.\n", header.file_size);
    printf("Chunk size: %d bytes.\n", header.chunk_size);

    char* buffer = (char*)malloc(header.chunk_size);
    if (!buffer) {
        fprintf(stderr, "Error: Memory allocation failed.\n");
        return;
    }

    FILE* fp = fopen(output_file_path, "wb");
    if (!fp) {
        fprintf(stderr, "Error: File open failed.\n");
        free(buffer);
        return;
    }

    size_t received_size = 0;
    int chunk_count = (file_size + chunk_size - 1) / chunk_size;
    int current_chunk = 0;

    while (received_size < file_size) {
        int size = zmq_recv(zsock_resolve(client_sock), buffer, chunk_size, 0);
        if (size == -1) {
            fprintf(stderr, "Error receiving data: %s\n", zmq_strerror(errno));
            break;
        }

        size_t written = fwrite(buffer, 1, size, fp);
        if (written < size) {
            fprintf(stderr, "Error writing to file: expected %d bytes, wrote %zu bytes\n", size, written);
            break;
        }
        if (zstr_send(client_sock, "ACK") == -1) {
            fprintf(stderr, "Error sending acknowledgment: %s\n", zmq_strerror(errno));
        }
        received_size += size;
        current_chunk++;
        printf("Chunk %d/%d received. Size: %d bytes.\n", current_chunk, chunk_count, size);
    }
    free(buffer);
    fclose(fp);
    
    // Compute hash of received file and compare with the expected hash
    unsigned char* expected_hash = header.hash;
    unsigned char received_hash[SHA512_DIGEST_LENGTH];
    compute_sha512(output_file_path, received_hash); // Compute hash of received file
    char received_hash_string[SHA512_DIGEST_LENGTH * 2 + 1];
    char expected_hash_string[SHA512_DIGEST_LENGTH * 2 + 1];
    convert_hash_to_hex_string(received_hash, received_hash_string, SHA512_DIGEST_LENGTH); // Convert received hash
    convert_hash_to_hex_string(expected_hash, expected_hash_string, SHA512_DIGEST_LENGTH); // Convert expected hash

    // Compare the raw binary hashes
    if (memcmp(received_hash, expected_hash, SHA512_DIGEST_LENGTH) != 0) 
    {
        fprintf(stderr, "Received file hash does not match\n");
        printf("Received Hash (binary): ");
        for (int i = 0; i < SHA512_DIGEST_LENGTH; i++) 
        {
            printf("%02x", received_hash[i]);
        }
        printf("\nExpected Hash (binary): ");
        for (int i = 0; i < SHA512_DIGEST_LENGTH; i++) 
        {
            printf("%02x", expected_hash[i]);
        }
        printf("\n");
    }
    else {
        printf("File hash verified successfully.\n");
    }
}

/**
 * @brief The main function for the client.
 *
 * Initializes the ZeroMQ context, creates a client socket, connects to the server, and receives a file via the socket.
 * Compares the received file's hash with the expected hash for integrity verification.
 *
 * @param argc The number of command-line arguments.
 * @param argv The array of command-line arguments.
 * @return An integer representing the exit status.
 */
int client_main(int argc, char const* argv[]) {
	if (argc < 4)
	{
		printf("Usage: %s client [port] [threads] [output file path]\n", argv[0]);
		return 1;
	}

	const char* port = argv[1];
	int threads = atoi(argv[2]);
	const char* output_file_path = argv[3];

    printf("Connecting to server at port %s...\n", port);

	void* context = zmq_ctx_new();
	assert(context);

	// Create and bind the PULL socket
	zsock_t* client_sock = client(context, port, threads);
	assert(client_sock);

	// Receive file
	client_receive(client_sock, output_file_path);
    printf("\x1b[32mFile transfer completed successfully.\x1b[32m\n ");
    // return to normal color
    printf("\x1b[0m");

	// Clean up
	zsock_destroy(&client_sock);
	zmq_ctx_destroy(&context);

	return 0;
}