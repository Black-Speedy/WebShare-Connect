/**
 * @file receiver_main.c
 * @brief Main receiver functionality.
 */

#include "WebShare-Connect.h"
#include "receiver.h"
#include "sha512.h"
#include "nice.h"

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
 * @brief Sets up a receiver socket and connects to the sender.
 *
 * Initializes and connects a receiver socket to the specified sender port.
 *
 * @param context The ZeroMQ context.
 * @param port The port of the sender to connect to.
 * @param threads The number of threads to use.
 * @return A pointer to the created receiver socket.
 */
zsock_t* receiver(void* context, const char* ip_address, const char* port, int threads)
{
    //string ip adress
    int io_threads = threads;
    zmq_ctx_set(context, ZMQ_IO_THREADS, io_threads);
    assert(zmq_ctx_get(context, ZMQ_IO_THREADS) == io_threads);

    printf("Creating receiver socket in receiver()\n");
    zsock_t* receiver_sock = zsock_new(ZMQ_PAIR);
    assert(receiver_sock);
    printf("Assert 2\n");

    int rc = zsock_connect(receiver_sock, "tcp://%s:%s", ip_address, port); // change to to sender ip when running on different machines
    assert(rc != -1);
    printf("Assert 3\n");

    zsock_set_rcvtimeo(receiver_sock, 3000); // 2s timeout for recv

    return receiver_sock;
}

/**
 * @brief Receives a file via the receiver socket.
 *
 * Receives the file header, then receives file chunks and writes them to the specified output file path.
 * Computes the hash of the received file and compares it with the expected hash.
 *
 * @param receiver_sock The receiver socket used for receiving the file.
 * @param output_file_path The path to save the received file.
 */
void receiver_receive(zsock_t* receiver_sock, const char* output_file_path) {
    printf("\n-- File Reception --\n");

    file_header_t header;
    size_t header_size = sizeof(file_header_t);
    byte* header_buf = NULL;

    // Receive the header
    printf("Receiving header");
    int rc = zsock_recv(receiver_sock, "b", &header_buf, &header_size);
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
        int size = zmq_recv(zsock_resolve(receiver_sock), buffer, chunk_size, 0);
        if (size == -1) {
            fprintf(stderr, "Error receiving data: %s\n", zmq_strerror(errno));
            break;
        }

        size_t written = fwrite(buffer, 1, size, fp);
        if (written < size) {
            fprintf(stderr, "Error writing to file: expected %d bytes, wrote %zu bytes\n", size, written);
            break;
        }
        if (zstr_send(receiver_sock, "ACK") == -1) {
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
 * @brief The main function for the receiver.
 *
 * Initializes the ZeroMQ context, creates a receiver socket, connects to the sender, and receives a file via the socket.
 * Compares the received file's hash with the expected hash for integrity verification.
 *
 * @param argc The number of command-line arguments.
 * @param argv The array of command-line arguments.
 * @return An integer representing the exit status.
 */
int receiver_main(int argc, char const* argv[]) {
    if (argc < 4)
    {
        printf("Usage: %s receiver [port] [threads] [output file path]\n", argv[0]);
        return 1;
    }

    // temporary ip address should come in argument
    const char* ip_address = "192.168.0.69";
    const char* port = argv[1];
    int threads = atoi(argv[2]);
    const char* output_file_path = argv[3];

    printf("Connecting to sender at port %s...\n", port);

    void* context = zmq_ctx_new();
    assert(context);
    printf("Assert 1\n");

    // Create and bind the PULL socket
    printf("Creating receiver socket\n");
    zsock_t* receiver_sock = receiver(context, ip_address, port, threads);
    assert(receiver_sock);
    printf("Assert 4\n");

    // Receive file
    receiver_receive(receiver_sock, output_file_path);
    printf("\x1b[32mFile transfer completed successfully.\x1b[32m\n ");
    // return to normal colorr
    printf("\x1b[0m\n");

    printf("Press Enter to exit.\n");
    char x = getchar();
    scanf("%c", &x) ? x : x;

    // Clean up
    zsock_destroy(&receiver_sock);
    zmq_ctx_destroy(&context);

    return 0;
}
