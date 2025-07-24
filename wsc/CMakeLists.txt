cmake_minimum_required(VERSION 3.16)

# project(wsc LANGUAGES C)

add_executable(wsc main.c
               handlearguments.c
               options.c

)

message(STATUS "Super project version: ${PROJECT_VERSION}")


include_directories(${CMAKE_SOURCE_DIR}/core)
include_directories(${CMAKE_BINARY_DIR}/core)

target_link_libraries(wsc PRIVATE core)

include(GNUInstallDirs)
install(TARGETS wsc
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
