cmake_minimum_required(VERSION 3.16)
project(WebShare-Connect_CLI LANGUAGES C VERSION 0.0.1)

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
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../Include)

# Add sender executable to the project.
add_executable(WSC 
    "WebShare-Connect.c" 
    "WebShare-Connect.h"
)

add_library(terminalProgressBar "terminalProgressBar.c" "terminalProgressBar.h")

# Also build the core libraries
include_directories("../Core")

# Include the OpenSSL include directory
include_directories(${OPENSSL_INCLUDE_DIR})

# Link against libraries
target_link_libraries(WSC PRIVATE 
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

target_link_libraries(terminalProgressBar PRIVATE)

# Set C standard
set_property(TARGET WSC PROPERTY C_STANDARD 11)

# TODO: Add tests and install targets if needed.
