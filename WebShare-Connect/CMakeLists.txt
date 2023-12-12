# Add source to this project's executable.
include_directories("R:/vcpkg/packages/czmq_x64-windows/include/")

# Add server executable to the project.
add_executable (Server "WebShare-Connect.c" "WebShare-Connect.h") # Adjust "server.c" to your server source file

# Add client executable to the project.
add_executable (Client "client.c" "WebShare-Connect.h") # Adjust "client.c" to your client source file

find_package(czmq CONFIG REQUIRED)
find_package(ZeroMQ CONFIG REQUIRED)

# Link libraries to your server executable.
target_link_libraries(Server PRIVATE czmq)
target_link_libraries(Server PRIVATE libzmq)

# Link libraries to your client executable.
target_link_libraries(Client PRIVATE czmq)
target_link_libraries(Client PRIVATE libzmq)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Server PROPERTY C_STANDARD 11)
  set_property(TARGET Client PROPERTY C_STANDARD 11)
endif()

# TODO: Add tests and install targets if needed.
