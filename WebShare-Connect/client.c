#include "WebShare-Connect.h"
#include "client.h"

// This is the header for the file
typedef struct {
	int file_size;
	int chunk_size;
} file_header_t;

zsock_t* client(void* context, const char *port, int threads)
{
	int io_threads = threads;
	zmq_ctx_set(context, ZMQ_IO_THREADS, io_threads);
	assert(zmq_ctx_get(context, ZMQ_IO_THREADS) == io_threads);

	zsock_t* client_sock = zsock_new(ZMQ_PULL);
	assert(client_sock);

	int rc = zsock_connect(client_sock, "tcp://127.0.0.1:%s", port); // change to to server ip when running on different machines
	assert(rc != -1);

	zsock_set_rcvtimeo(client_sock, 2000); // 2s timeout for recv

	return client_sock;
}

void client_receive(zsock_t* client_sock, const char* output_file_path) {
    printf("Receiving file...\n");

    file_header_t header;
    size_t header_size = sizeof(file_header_t);
    byte* header_buf = NULL;

    // Receive the header as a binary blob
    int rc = zsock_recv(client_sock, "b", &header_buf, &header_size);
    if (rc == -1) {
        fprintf(stderr, "Failed to receive header\n");
        return;
    }

    // Ensure the received header size matches the expected size
    if (header_size != sizeof(file_header_t)) {
        fprintf(stderr, "Received header size does not match\n");
        free(header_buf); // Free the received buffer
        return;
    }

    // Copy the received data into the header structure
    if (header_buf == NULL) {
		fprintf(stderr, "Received header is NULL\n");
		return;
	}
    memcpy(&header, header_buf, sizeof(header));
    free(header_buf); // Free the received buffer after copying

    printf("Received file header\n");
    int file_size = header.file_size;
    int chunk_size = header.chunk_size;
    printf("Expected file size: %d bytes\n", file_size);
    printf("Chunk size: %d bytes\n", chunk_size);

    char* buffer = (char*)malloc(chunk_size);
    if (buffer == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        return;
    }

    FILE* fp = fopen(output_file_path, "wb");
    if (fp == NULL) {
        fprintf(stderr, "File open error\n");
        free(buffer);
        return;
    }

    size_t received_size = 0;
    int chunk_count = (file_size + chunk_size - 1) / chunk_size;
    int current_chunk = 0;

    while (received_size < file_size) {
        size_t size;
        if (zsock_recv(client_sock, "b", &buffer, &size, file_size) == -1) { // Receive a chunk of data from the server socket and store it in the buffer variable (with a maximum size of chunk_size)
            fprintf(stderr, "Error receiving data\n");
            break;
        }

        size_t written = fwrite(buffer, 1, size, fp);
        if (written < size) {
            fprintf(stderr, "Error writing to file: expected %zu, wrote %zu\n", size, written);
            break;
        }

        received_size += size;
        current_chunk++;
        printf("Received and wrote chunk %zu bytes\n", written);
    }

    free(buffer);
    fclose(fp);
    printf("File reception completed.\n");
}


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
    printf("File received.\n");

	// Clean up
	zsock_destroy(&client_sock);
	zmq_ctx_destroy(&context);

	return 0;
}