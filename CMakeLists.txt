cmake_minimum_required(VERSION 3.16)

set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 0)
set(PROJECT_VERSION_PATCH 1)
set(PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")

option(BUILD_TESTS "Build the tests" ON)

project(WebShare-Connect 
        VERSION ${PROJECT_VERSION} 
        LANGUAGES C CXX)

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

# Check if all required Qt6 components are available
find_package(Qt6 REQUIRED)
if(Qt6_FOUND)
    message("Qt6 Found: " ${Qt6_FOUND})
    message("Qt6 Version: ${Qt6_VERSION}")
endif()



# # this is heuristically generated, and may not be correct
# find_package(Qt6 CONFIG REQUIRED)
# # note: 37 additional targets are not displayed.
# target_link_libraries(main PRIVATE DB2::DB2 PPS::PPS Oracle::OCI Libb2::Libb2)

# find_package(Qt6Concurrent CONFIG REQUIRED)
# target_link_libraries(main PRIVATE Qt6::Concurrent Qt6::ConcurrentPrivate)

# find_package(Qt6Core CONFIG REQUIRED)
# target_link_libraries(main PRIVATE Qt6::Core Qt6::CorePrivate)

# find_package(Qt6DBus CONFIG REQUIRED)
# target_link_libraries(main PRIVATE Qt6::DBus Qt6::DBusPrivate)

# find_package(Qt6DeviceDiscoverySupportPrivate CONFIG REQUIRED)
# target_link_libraries(main PRIVATE Qt6::DeviceDiscoverySupportPrivate)

# find_package(Qt6EntryPointPrivate CONFIG REQUIRED)
# target_link_libraries(main PRIVATE Qt6::EntryPointPrivate Qt6::EntryPointImplementation)

# find_package(Qt6ExampleIconsPrivate CONFIG REQUIRED)
# target_link_libraries(main PRIVATE Qt6::ExampleIconsPrivate Qt6::ExampleIconsPrivate_resources_1)

# find_package(Qt6ExamplesAssetDownloaderPrivate CONFIG REQUIRED)
# target_link_libraries(main PRIVATE Qt6::ExamplesAssetDownloaderPrivate)

# find_package(Qt6FbSupportPrivate CONFIG REQUIRED)
# target_link_libraries(main PRIVATE Qt6::FbSupportPrivate)

# find_package(Qt6Gui CONFIG REQUIRED)
# # note: 6 additional targets are not displayed.
# target_link_libraries(main PRIVATE Qt6::Gui Qt6::GuiPrivate Qt6::QGifPlugin Qt6::QICOPlugin)

# find_package(Qt6Network CONFIG REQUIRED)
# # note: 2 additional targets are not displayed.
# target_link_libraries(main PRIVATE Qt6::Network Qt6::QNLMNIPlugin Qt6::NetworkPrivate Qt6::QSchannelBackendPlugin)

# find_package(Qt6OpenGL CONFIG REQUIRED)
# target_link_libraries(main PRIVATE Qt6::OpenGL Qt6::OpenGLPrivate)

# find_package(Qt6OpenGLWidgets CONFIG REQUIRED)
# target_link_libraries(main PRIVATE Qt6::OpenGLWidgets Qt6::OpenGLWidgetsPrivate)

# find_package(Qt6PrintSupport CONFIG REQUIRED)
# target_link_libraries(main PRIVATE Qt6::PrintSupport Qt6::PrintSupportPrivate)

# find_package(Qt6Sql CONFIG REQUIRED)
# target_link_libraries(main PRIVATE Qt6::Sql Qt6::SqlPrivate Qt6::QPSQLDriverPlugin Qt6::QSQLiteDriverPlugin)

# find_package(Qt6Test CONFIG REQUIRED)
# target_link_libraries(main PRIVATE Qt6::Test Qt6::TestPrivate)

# find_package(Qt6TestInternalsPrivate CONFIG REQUIRED)
# target_link_libraries(main PRIVATE Qt6::TestInternalsPrivate)

# find_package(Qt6Widgets CONFIG REQUIRED)
# target_link_libraries(main PRIVATE Qt6::Widgets Qt6::WidgetsPrivate Qt6::QModernWindowsStylePlugin)

# find_package(Qt6Xml CONFIG REQUIRED)
# target_link_libraries(main PRIVATE Qt6::Xml Qt6::XmlPrivate)





include(GNUInstallDirs)
install(TARGETS WebShare-Connect
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

configure_file(
    ${CMAKE_SOURCE_DIR}/core/version.h.in
    ${CMAKE_BINARY_DIR}/core/version.h
    @ONLY
)

add_subdirectory(core)
add_subdirectory(wsc)
add_subdirectory(gui)

# Add tests if enabled
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

target_link_libraries(WebShare-Connect PRIVATE core)