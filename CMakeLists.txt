cmake_minimum_required(VERSION 3.16)

project(WebShare-Connect LANGUAGES C)

add_executable(WebShare-Connect main.c
    CMakePresets.json)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

message("Finding library ZeroMQ / libzmq")
find_package(ZeroMQ CONFIG REQUIRED)
if(ZeroMQ_FOUND)
    message("ZeroMQ Found: TRUE")
    message("ZeroMq Version: ${ZeroMQ_VERSION}")
endif()
message("Finding library czmq")
find_package(czmq CONFIG REQUIRED)
if(czmq_FOUND)
    message("czmq Found: TRUE")
    message("czmq Version: ${czmq_VERSION}")
endif()
message("Finding library OpenSSL")
find_package(OpenSSL REQUIRED)
if(OPENSSL_FOUND)
    message("OPENSSL Found: " ${OPENSSL_FOUND})
    message("OPENSSL Version: ${OPENSSL_VERSION}")
endif()
message("Finding library LibNice")
find_package(LibNice REQUIRED)
if(LibNice_FOUND)
    message("LibNice Found: " ${LibNice_FOUND})
    message("LibNice Version: ${LibNice_VERSION}")
endif()
find_package(Qt6 REQUIRED)
if(Qt6_FOUND)
    message("Qt6 Found: " ${Qt6_FOUND})
    message("Qt6 Version: ${Qt6_VERSION}")
endif()

include(GNUInstallDirs)
install(TARGETS WebShare-Connect
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
add_subdirectory(core)
add_subdirectory(wsc)
add_subdirectory(gui)
