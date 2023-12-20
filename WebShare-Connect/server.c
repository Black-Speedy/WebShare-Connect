#include "WebShare-Connect.h"
#include "server.h"
#include "sha512.h"

void server(char* client_ip, char* port, int threads)
{
	int io_threads = threads;
	void* context = zmq_ctx_new();
	zmq_ctx_set(context, ZMQ_IO_THREADS, io_threads);
	assert(zmq_ctx_get(context, ZMQ_IO_THREADS) == io_threads);
	
	zsock_t *serv_sock = zsock_new(ZMQ_PUSH);
	assert(serv_sock);

	int rc = zsock_bind(serv_sock, "tcp://*:%s", port);
	assert(rc != -1);

	zsock_set_rcvtimeo(serv_sock, 5000); // 5s timeout for recv
}

void server_send(zsock_t* serv_sock, const char* file_path)
{
	// Open file
	FILE *fp = fopen(file_path, "rb");
	if (fp == NULL)
	{
		printf("File open error\n");
		return;
	}

	// Determine file size
	fseek(fp, 0L, SEEK_END);
	int file_size = ftell(fp);
	rewind(fp); 

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
		chunk_size = 2 ^ 24;
	}

	// Send file size
	char* buffer = (char*)malloc(chunk_size);
	if (buffer == NULL)
	{
		printf("Failed to allocate memory\n");
		fclose(fp);
		return;
	}

	// Send file size until end of file
	while (!feof(fp)) {
		size_t bytesRead = fread(buffer, 1, chunk_size, fp);
		if (ferror(fp)) {
			perror("Error reading file");
			break;
		}
		zsock_send(serv_sock, "b", buffer, bytesRead); // Send file chunk
	}

	free(buffer);
	fclose(fp);
	
	// Send end of file
	zstr_send(serv_sock, "EOF");
}

// TODO: Maybe multithread the computing of the hash and sending of the file chunks. 
// So the server can send the file chunks while the hash is being computed.