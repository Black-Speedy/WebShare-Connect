#include "WebShare-Connect.h"
#include "server.h"
#include "sha512.h"

// This is the header for the file
typedef struct {
	int64_t file_size;
	int chunk_size;
	unsigned char hash[SHA512_DIGEST_LENGTH];
} file_header_t;

zsock_t* server(void* context, const char *port, int threads)
{
	int io_threads = threads;
	zmq_ctx_set(context, ZMQ_IO_THREADS, io_threads);
	assert(zmq_ctx_get(context, ZMQ_IO_THREADS) == io_threads);
	
	zsock_t* serv_sock = zsock_new(ZMQ_PAIR);
	assert(serv_sock);

	int rc = zsock_bind(serv_sock, "tcp://*:%s", port);
	assert(rc != -1);

	zsock_set_rcvtimeo(serv_sock, 2000); // 2s timeout for recv

	return serv_sock;
}

void server_send(zsock_t* serv_sock, const char* file_path)
{
	// Open file
	FILE* fp = fopen(file_path, "rb");
	if (!fp) {
		printf("Error: File open failed.\n");
		return;
	}

	// Determine file size
	fseek(fp, 0L, SEEK_END);
	int64_t file_size = _ftelli64(fp);
	rewind(fp);
	if (file_size < 1) {
		printf("Error: File size error.\n");
		fclose(fp);
		return;
	}

	printf("  File size: %lld bytes\n", file_size);

	int chunk_size;

	if (file_size == 0)
	{
		printf("File is empty\n");
		return;
	}
	else if (file_size <= FILE_SIZE_1MB) 
	{
		chunk_size = CHUNK_SIZE_1MB;
	}
	else if (file_size <= FILE_SIZE_100MB) 
	{
		chunk_size = CHUNK_SIZE_100MB;
	}
	else if (file_size <= FILE_SIZE_1GB)
	{
		chunk_size = CHUNK_SIZE_1GB;
	}
	else if (file_size <= FILE_SIZE_10GB)
	{
		chunk_size = CHUNK_SIZE_10GB;
	}
	else if (file_size <= FILE_SIZE_20GB)
	{
		chunk_size = CHUNK_SIZE_20GB;
	}
	else if (file_size <= FILE_SIZE_35GB)
	{
		chunk_size = CHUNK_SIZE_35GB;
	}
	else if (file_size <= FILE_SIZE_50GB)
	{
		chunk_size = CHUNK_SIZE_50GB;
	}
	else {
		chunk_size = pow(2, 24);
	}

	printf("  Chunk size: %d bytes\n", chunk_size);

	// Compute hash
	unsigned char hash[SHA512_DIGEST_LENGTH];
	compute_sha512(file_path, hash);

	file_header_t header;
	header.file_size = file_size;
	header.chunk_size = chunk_size;
	memcpy(header.hash, hash, SHA512_DIGEST_LENGTH);

	// Convert hash to hex string
	char hash_string[SHA512_DIGEST_LENGTH * 2 + 1];
	convert_hash_to_hex_string(hash, hash_string, SHA512_DIGEST_LENGTH);
	printf("  Hash: %s\n", hash_string);

	// Send file header
	zsock_send(serv_sock, "b", &header, sizeof(header));

	// Allocate buffer and send file
	char* buffer = (char*)malloc(chunk_size);
	if (!buffer) {
		printf("Error: Memory allocation failed.\n");
		fclose(fp);
		return;
	}

	int64_t chunk_count = file_size / (int64_t)chunk_size + (file_size % (int64_t)chunk_size != 0);
	int current_chunk = 0;
	printf("  Total chunks to send: %lld\n", chunk_count);

	while (1) {
		memset(buffer, 0, chunk_size); // Clear the buffer before reading
		size_t bytesRead = fread(buffer, 1, chunk_size, fp);
		if (bytesRead == 0) {
			if (ferror(fp)) {
				perror("Error reading file");
			}
			break; // Break the loop if no more data to read
		}
		zsock_send(serv_sock, "b", buffer, bytesRead); // Send file chunk

		current_chunk++;
		if (current_chunk % 100 == 0)
		{
			printf("Sent chunk %d/%lld with size %lld bytes\n", current_chunk, chunk_count, bytesRead);
		}
		
		
		char* ack = zstr_recv(serv_sock);
		if (!ack) {
			fprintf(stderr, "Failed to receive acknowledgment: %s\n", zmq_strerror(errno));
			break;
		}
		if (strcmp(ack, "ACK") != 0) {
			fprintf(stderr, "Received incorrect acknowledgment\n");
			free(ack);
			break;
		}
		free(ack);
	}

	free(buffer);
	fclose(fp);
}

int server_main(int argc, char const* argv[]) {
	if (argc < 4) 
	{
		printf("Usage: %s server [port] [threads] [file_path]\n", argv[-1]);
		return 1;
	}

	const char* port = argv[1];
	int threads = atoi(argv[2]);
	const char* file_path = argv[3];

	void* context = zmq_ctx_new();
	assert(context);

	// Create and bind the PUSH socket
	zsock_t* serv_sock = server(context, port, threads);
	assert(serv_sock);

	// Send the file
	server_send(serv_sock, file_path);

	// Clean up
	zsock_destroy(&serv_sock);
	zmq_ctx_destroy(&context);

	return 0;
}

// TODO: Maybe multithread the computing of the hash and sending of the file chunks. 
// So the server can send the file chunks while the hash is being computed.