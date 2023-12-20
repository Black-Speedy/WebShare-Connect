#define CHUNK_SIZE_1MB	(1 << 12)         // 4 KiB
#define CHUNK_SIZE_100MB (1 << 16)       // 64 KiB
#define CHUNK_SIZE_1GB	(1 << 18)         // 256 KiB
#define CHUNK_SIZE_10GB (1 << 20)        // 1 MiB
#define CHUNK_SIZE_20GB (1 << 21)        // 2 MiB
#define CHUNK_SIZE_35GB ((1 << 21) + (1 << 20)) // 3 MiB
#define CHUNK_SIZE_50GB (1 << 22)        // 4 MiB

#define FILE_SIZE_1MB   1 * 1000 * 1000LL // 1 MiB
#define FILE_SIZE_100MB 100 * 1000 * 1000LL // 100 MiB
#define FILE_SIZE_1GB   1 * 1000 * 1000 * 1000LL // 1 GiB
#define FILE_SIZE_10GB 10 * 1000 * 1000 * 1000LL // 10 GiB
#define FILE_SIZE_20GB 20 * 1000 * 1000 * 1000LL // 20 GiB
#define FILE_SIZE_35GB 35 * 1000 * 1000 * 1000LL // 35 GiB
#define FILE_SIZE_50GB 50 * 1000 * 1000 * 1000LL // 50 GiB

zsock_t* server(void* context, const char* port, int threads);
void server_send(zsock_t* serv_sock, const char* output_file_path);
int server_main(int argc, char const* argv[]);