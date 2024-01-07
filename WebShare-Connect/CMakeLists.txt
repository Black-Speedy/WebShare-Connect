# Add include directories to your project.
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Include)

# Add server executable to the project.
add_executable (WebShare-Connect "WebShare-Connect.c" "WebShare-Connect.h") # Adjust "server.c" to your server source file

add_library (sha512 "sha512.c" "sha512.h")
add_library (server "server.c" "server.h")
add_library (client "client.c" "client.h")
add_library (removeQuotes "removeQuotes.c" "removeQuotes.h")

find_package(czmq CONFIG REQUIRED)
find_package(ZeroMQ CONFIG REQUIRED)
find_package(OpenSSL REQUIRED)

# Set the path to GLib include directory
set(GLIB_INCLUDE_DIR "C:/PackageManagers/vcpkg/installed/x64-windows/include/glib-2.0")
# Include GLib directories
include_directories(${GLIB_INCLUDE_DIR})
include_directories("C:/PackageManagers/vcpkg/installed/x64-windows/lib/glib-2.0/include")  # Path to glibconfig.h

add_definitions(-DG_OS_WIN32)

# Set the path to libnice include directory
set(LIBNICE_INCLUDE_DIR "C:/PackageManagers/vcpkg/installed/x64-windows/include")

# Include libnice directories
include_directories(${LIBNICE_INCLUDE_DIR})
include_directories("${LIBNICE_INCLUDE_DIR}/nice")  # Path to the 'nice' subdirectory containing address.h


# Link against libnice library
target_link_libraries(WebShare-Connect PRIVATE "C:/PackageManagers/vcpkg/installed/x64-windows/lib/glib-2.0.lib")
target_link_libraries(WebShare-Connect PRIVATE "C:/PackageManagers/vcpkg/installed/x64-windows/lib/nice.lib")



# Link libraries to your server executable.
target_link_libraries(WebShare-Connect PRIVATE czmq)
target_link_libraries(WebShare-Connect PRIVATE libzmq)
target_link_libraries(WebShare-Connect PRIVATE sha512)
target_link_libraries(WebShare-Connect PRIVATE server)
target_link_libraries(WebShare-Connect PRIVATE client)
target_link_libraries(WebShare-Connect PRIVATE OpenSSL::SSL OpenSSL::Crypto)
target_link_libraries(WebShare-Connect PRIVATE removeQuotes)
target_link_libraries(server PRIVATE czmq)
target_link_libraries(server PRIVATE libzmq)
target_link_libraries(sha512 PRIVATE OpenSSL::SSL OpenSSL::Crypto)
target_link_libraries(client PRIVATE czmq)


# Link libraries to your client executable.
if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET WebShare-Connect PROPERTY C_STANDARD 11)
endif()

# TODO: Add tests and install targets if needed.