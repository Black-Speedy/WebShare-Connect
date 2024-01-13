# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

if(WIN32)
    # Set the vcpkg toolchain file if vcpkg is being used
    if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
      set(CMAKE_TOOLCHAIN_FILE "-DCMAKE_TOOLCHAIN_FILE=R:/vcpkg/scripts/buildsystems/vcpkg.cmake"
          CACHE STRING "")
    endif()
endif()
# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project(WebShare-Connect
        LANGUAGES C
        VERSION 0.0.1)


set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
find_package(LibNice REQUIRED)
message("LibNice_FOUND: ${LibNice_FOUND}")
if(WIN32)
    find_package(ZeroMQ CONFIG REQUIRED)
    message("ZeroMQ_FOUND: ${ZeroMQ_FOUND}")
    message("ZeroMQ_VERSION: ${ZeroMQ_VERSION}")
    find_package(czmq CONFIG REQUIRED)
    message("czmq_FOUND: ${czmq_FOUND}")
message("czmq_VERSION: ${czmq_VERSION}")
elseif(UNIX AND NOT APPLE)

    find_package(PkgConfig REQUIRED)
    message(STATUS "PkgConfig found: ${PKG_CONFIG_FOUND}")

    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "/usr/local/lib/cmake/")

    set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH}"/usr/include")
    pkg_check_modules(czmq REQUIRED libczmq)
    message("CZMQ Dir" ${czmq_INCLUDE_DIRS})
    message("CZMQ Lib" ${czmq_LIBRARIES})
    message("czmq_FOUND: ${czmq_FOUND}")
    message("czmq_VERSION: ${czmq_VERSION}")

    set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH}"/usr/include")
    find_package(ZeroMQ CONFIG REQUIRED)
    message("ZeroMQ Dir" ${ZeroMQ_INCLUDE_DIRS})
    message("ZeroMQ Lib" ${ZeroMQ_LIBRARIES})
    message("ZeroMQ_FOUND: ${ZeroMQ_FOUND}")
    message("ZeroMQ_VERSION: ${ZeroMQ_VERSION}")
endif()
find_package(OpenSSL REQUIRED)
#set(OPENSSL_INCLUDE_DIR "${OPENSSL_INCLUDE_DIR}/openssl")
message("OPENSSL_FOUND: ${OPENSSL_FOUND}")



# Include sub-projects.
add_subdirectory ("WebShare-Connect")

# Link libraries to your project (replace 'your_target_name' with the actual target name)
# target_link_libraries(your_target_name PRIVATE czmq ZeroMQ::ZeroMQ)
