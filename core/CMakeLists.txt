cmake_minimum_required(VERSION 3.16)
project(core LANGUAGES C)

add_library(core
    core.c
    core.h
)

target_link_libraries(core PRIVATE libzmq libzmq-static)

target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

include(GNUInstallDirs)
install(TARGETS core
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
