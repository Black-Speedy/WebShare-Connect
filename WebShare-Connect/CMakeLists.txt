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

add_definitions(-DG_OS_WIN32)
# Set include directories
set(INCLUSIONS "${_VCPKG_INSTALLED_DIR}/x64-windows/include")
set(LIBRARIES  "${_VCPKG_INSTALLED_DIR}/x64-windows/lib")

set(GLIB_INCLUDE_DIR "${INCLUSIONS}/glib-2.0")
set(GLIB_LIB_DIR "${LIBRARIES}/glib-2.0.lib")
set(GLIB_CONF "${LIBRARIES}/glib-2.0/include")

set(LIBNICE_INCLUDE_DIR "${INCLUSIONS}/nice")
set(LIBNICE_LIB_DIR "${LIBRARIES}/nice.lib")

# Include directories
include_directories(${GLIB_INCLUDE_DIR}) 
include_directories(${GLIB_LIB_DIR})
include_directories(${GLIB_CONF})
include_directories(${LIBNICE_INCLUDE_DIR})

# Link against libraries
target_link_libraries(WebShare-Connect PRIVATE 
    ${GLIB_LIB_DIR} # Link against the actual GLib library file
    ${LIBNICE_LIB_DIR}
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

target_link_libraries(sha512 PRIVATE OpenSSL::SSL OpenSSL::Crypto)

# Set C standard
set_property(TARGET WebShare-Connect PROPERTY C_STANDARD 11)

# TODO: Add tests and install targets if needed.
