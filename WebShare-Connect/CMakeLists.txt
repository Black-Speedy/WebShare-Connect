cmake_minimum_required(VERSION 3.12)
project(WebShare-Connect)

# Add include directories to your project.
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Include)

# Add server executable to the project.
add_executable(WebShare-Connect 
    "WebShare-Connect.c" 
    "WebShare-Connect.h"
)

# Add libraries
add_library(sha512 "sha512.c" "sha512.h")
add_library(server "server.c" "server.h")
add_library(client "client.c" "client.h")
add_library(removeQuotes "removeQuotes.c" "removeQuotes.h")

# Find packages
find_package(czmq CONFIG REQUIRED)
find_package(ZeroMQ CONFIG REQUIRED)
find_package(OpenSSL REQUIRED)

# Set Vcpkg directory
set(_VCPKG_INSTALLED_DIR "C:/PackageManagers/vcpkg/installed")

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
    server 
    client 
    OpenSSL::SSL 
    OpenSSL::Crypto 
    removeQuotes
)

# Link libraries to server and client targets
target_link_libraries(server PRIVATE czmq libzmq sha512 OpenSSL::SSL OpenSSL::Crypto)
target_link_libraries(client PRIVATE czmq)

# Set C standard
set_property(TARGET WebShare-Connect PROPERTY C_STANDARD 11)

# TODO: Add tests and install targets if needed.
