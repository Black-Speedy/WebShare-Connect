cmake_minimum_required(VERSION 3.16)
project(WebShare-Connect-Dependencies LANGUAGES C VERSION 0.0.1)

# Add libraries
add_library(sha512 "sha512.c" "sha512.h")
# Disable all warnings for the sha512 library
if (MSVC)
    target_compile_options(sha512 PRIVATE /w)
else()
    target_compile_options(sha512 PRIVATE -w)
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../Include)

add_library(sender "sender.c" "sender.h")
add_library(receiver "receiver.c" "receiver.h")
add_library(removeQuotes "removeQuotes.c" "removeQuotes.h")
add_library(fileChunks "../Include/fileChunks.c" "../Include/fileChunks.h")

# Build the dlls
# Link libraries to sender and receiver targets
target_link_libraries(sender PRIVATE 
    czmq 
    libzmq
    sha512 
    OpenSSL::SSL 
    OpenSSL::Crypto
    LibNice::LibNice
    fileChunks
    terminalProgressBar
)

target_link_libraries(receiver PRIVATE 
    czmq
    libzmq
    LibNice::LibNice
    fileChunks
    terminalProgressBar
)

target_link_libraries(sha512 PRIVATE OpenSSL::SSL OpenSSL::Crypto)