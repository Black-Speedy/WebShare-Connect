cmake_minimum_required(VERSION 3.16)
project(core LANGUAGES C)

add_library(core
    core.c
    core.h
    cthreads.c
    cthreads.h
    jobqueue.c
    jobqueue.h
)

target_link_libraries(core PRIVATE libzmq)

target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

include(GNUInstallDirs)
install(TARGETS core
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
