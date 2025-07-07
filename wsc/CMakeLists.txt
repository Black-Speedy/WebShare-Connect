cmake_minimum_required(VERSION 3.16)

project(wsc LANGUAGES C)

add_executable(wsc main.c)

target_link_libraries(wsc PRIVATE core)

include(GNUInstallDirs)
install(TARGETS wsc
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

