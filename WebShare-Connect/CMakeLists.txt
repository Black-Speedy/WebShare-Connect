cmake_minimum_required(VERSION 3.12)
project(WebShare-Connect LANGUAGES C VERSION 0.0.1)

set(CMAKE_VERBOSE_MAKEFILE 1)

if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
  # Suppress warnings related to inline expansions
  set(CMAKE_C_FLAGS "/wd4710 /wd4711")
endif()

if (MSVC)
  # Use different flags for MSVC
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /Wall /w14242 /W4")
else()
  # Use your original flags for other compilers
  set(CMAKE_C_FLAGS "-Wall -Wextra -Wconversion -Wunreachable-code -Wshadow -Wundef")
endif()


# Add include directories to your project.
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Include)

# Add sender executable to the project.
add_executable(WebShare-Connect 
    "WebShare-Connect.c" 
    "WebShare-Connect.h"
)

# Add libraries
add_library(sha512 "sha512.c" "sha512.h")
# Disable all warnings for the sha512 library
if (MSVC)
    target_compile_options(sha512 PRIVATE /w)
else()
    target_compile_options(sha512 PRIVATE -w)
endif()

add_library(sender "sender.c" "sender.h")
add_library(receiver "receiver.c" "receiver.h")
add_library(removeQuotes "removeQuotes.c" "removeQuotes.h")
add_library(fileChunks "Include/fileChunks.c" "Include/fileChunks.h")
add_library(terminalProgressBar "terminalProgressBar.c" "terminalProgressBar.h")

# Include the OpenSSL include directory
include_directories(${OPENSSL_INCLUDE_DIR})

# Link against libraries
target_link_libraries(WebShare-Connect PRIVATE 
    sha512
    sender 
    receiver 
    OpenSSL::SSL 
    OpenSSL::Crypto 
    removeQuotes
    LibNice::LibNice
    fileChunks
    terminalProgressBar
    )

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

target_link_libraries(terminalProgressBar PRIVATE)

target_link_libraries(sha512 PRIVATE OpenSSL::SSL OpenSSL::Crypto)

# Set C standard
set_property(TARGET WebShare-Connect PROPERTY C_STANDARD 11)

# TODO: Add tests and install targets if needed.
