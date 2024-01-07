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
