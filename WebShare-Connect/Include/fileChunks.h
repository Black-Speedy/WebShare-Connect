#pragma once
#include <stdint.h>


/**
 * @brief Chunk sizes in bytes for various file sizes.
 */
#define CHUNK_SIZE_1MB      (1 << 12)               /**< Chunk size for    1 MB:   4 KiB */
#define CHUNK_SIZE_100MB    (1 << 16)               /**< Chunk size for  100 MB:  64 KiB */
#define CHUNK_SIZE_1GB      (1 << 18)               /**< Chunk size for    1 GB: 256 KiB */
#define CHUNK_SIZE_10GB     (1 << 20)               /**< Chunk size for   10 GB:   1 MiB */
#define CHUNK_SIZE_20GB     (1 << 21)               /**< Chunk size for   20 GB:   2 MiB */
#define CHUNK_SIZE_35GB     ((1 << 21) + (1 << 20)) /**< Chunk size for   35 GB:   3 MiB */
#define CHUNK_SIZE_50GB     (1 << 22)               /**< Chunk size for   50 GB:   4 MiB */
#define CHUNK_SIZE_BIG      (1 << 24)               /**< Chunk size for > 50 GB:  16 MiB */

/**
 * @brief File sizes in bytes for different capacities.
 */
#define FILE_SIZE_1MB       1 * 1000 * 1000LL         /**<   1 MB in bytes */
#define FILE_SIZE_100MB     100 * 1000 * 1000LL       /**< 100 MB in bytes */
#define FILE_SIZE_1GB       1 * 1000 * 1000 * 1000LL  /**<   1 GB in bytes */
#define FILE_SIZE_10GB      10 * 1000 * 1000 * 1000LL /**<  10 GB in bytes */
#define FILE_SIZE_20GB      20 * 1000 * 1000 * 1000LL /**<  20 GB in bytes */
#define FILE_SIZE_35GB      35 * 1000 * 1000 * 1000LL /**<  35 GB in bytes */
#define FILE_SIZE_50GB      50 * 1000 * 1000 * 1000LL /**<  50 GB in bytes */

typedef struct {
    int64_t max_file_size;                            // Maximum file size for this chunk size
    int     chunk_size;                               // Chunk size in bytes
} file_chunk_map_t;

/**
 * @brief Returns the appropriate chunk size based on file size.
 * @param file_size The file size in bytes.
 * @return The chunk size in bytes.
 */

int get_chunk_size(int64_t file_size);                // Ensure function prototype matches