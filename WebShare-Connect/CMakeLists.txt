cmake_minimum_required(VERSION 3.12)
project(WebShare-Connect)

# Add include directories to your project.
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Include)

# Add sender executable to the project.
add_executable(WebShare-Connect 
    "WebShare-Connect.c" 
    "WebShare-Connect.h"
)

# Add libraries
add_library(sha512 "sha512.c" "sha512.h")
add_library(sender "sender.c" "sender.h")
add_library(receiver "receiver.c" "receiver.h")
add_library(removeQuotes "removeQuotes.c" "removeQuotes.h")

# Find packages
find_package(czmq CONFIG REQUIRED)
find_package(ZeroMQ CONFIG REQUIRED)
find_package(OpenSSL REQUIRED)

# Set include directories
set(GLIB_INCLUDE_DIR "${_VCPKG_INSTALLED_DIR}/x64-windows/include/glib-2.0")
set(LIBNICE_INCLUDE_DIR "${_VCPKG_INSTALLED_DIR}/x64-windows/include")

# Include directories
include_directories(${GLIB_INCLUDE_DIR})
include_directories("${_VCPKG_INSTALLED_DIR}/x64-windows/lib/glib-2.0/include") 

include_directories(${LIBNICE_INCLUDE_DIR})
include_directories("${LIBNICE_INCLUDE_DIR}/nice")

# Link against libraries
target_link_libraries(WebShare-Connect PRIVATE 
    "${_VCPKG_INSTALLED_DIR}/x64-windows/lib/glib-2.0.lib"
    "${_VCPKG_INSTALLED_DIR}/x64-windows/lib/nice.lib"
    czmq 
    libzmq 
    sha512 
    sender 
    receiver 
    OpenSSL::SSL 
    OpenSSL::Crypto 
    removeQuotes
)

# Link libraries to sender and receiver targets
target_link_libraries(sender PRIVATE czmq libzmq sha512 OpenSSL::SSL OpenSSL::Crypto)
target_link_libraries(receiver PRIVATE czmq)

# Set C standard
set_property(TARGET WebShare-Connect PROPERTY C_STANDARD 11)

# TODO: Add tests and install targets if needed.
