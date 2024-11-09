#include "fileChunks.h"  // Includes file_chunk_map_t, chunk size constants, and function declaration

const file_chunk_map_t file_chunk_map[] = {
	{ FILE_SIZE_1MB,        CHUNK_SIZE_1MB },   // 1 MB: 4 KiB
	{ FILE_SIZE_100MB,      CHUNK_SIZE_100MB }, // 100 MB: 64 KiB
	{ FILE_SIZE_1GB,        CHUNK_SIZE_1GB },   // 1 GB: 256 KiB
	{ FILE_SIZE_10GB,       CHUNK_SIZE_10GB },  // 10 GB: 1 MiB
	{ FILE_SIZE_20GB,       CHUNK_SIZE_20GB },  // 20 GB: 2 MiB
	{ FILE_SIZE_35GB,       CHUNK_SIZE_35GB },  // 35 GB: 3 MiB
	{ FILE_SIZE_50GB,       CHUNK_SIZE_50GB }   // 50 GB: 4 MiB
};

int get_chunk_size(int64_t file_size) {
    for (int i = 0; i < sizeof(file_chunk_map) / sizeof(file_chunk_map[0]); i++) {
        if (file_size <= file_chunk_map[i].max_file_size) {
            return file_chunk_map[i].chunk_size;
        }
    }
    return CHUNK_SIZE_BIG;  // Default for files >50GB
}
