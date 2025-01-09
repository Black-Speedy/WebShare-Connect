cmake_minimum_required(VERSION 3.8)

if(WIN32)
    # Set the vcpkg toolchain file if vcpkg is being used
    if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
      set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
          CACHE STRING "")
    endif()
endif()

# Set the path for your custom libraries
set(LIBRARY "${CMAKE_SOURCE_DIR}/WebShare-Connect/Include/lib")
message("LIBRARY: ${LIBRARY}")

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project(WebShare-Connect
        LANGUAGES C
        VERSION 0.0.1)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
message("Finding library ZeroMQ / libzmq")
find_package(ZeroMQ CONFIG REQUIRED)
if(ZeroMQ_FOUND)
    message("\tZeroMQ Found: TRUE")
    message("\tZeroMq Version: ${ZeroMQ_VERSION}")
endif()

message("Finding library czmq")
find_package(czmq CONFIG REQUIRED)
if(czmq_FOUND)
	message("\tczmq Found: TRUE")
	message("\tczmq Version: ${czmq_VERSION}")
endif()

message("Finding library OpenSSL")
find_package(OpenSSL REQUIRED)
if(OPENSSL_FOUND)
	message("\tOPENSSL Found: " ${OPENSSL_FOUND})
	message("\tOPENSSL Version: ${OPENSSL_VERSION}")
endif()

message("Finding library LibNice")
find_package(LibNice REQUIRED)
if(LibNice_FOUND)
    message("\tLibNice Found: " ${LibNice_FOUND})
    message("\tLibNice Version: ${LibNice_VERSION}")
endif()

message("Finding library Sodium")
find_package(unofficial-sodium CONFIG REQUIRED)
if(unofficial-sodium_FOUND)
    message("\tSodium Found: " ${unofficial-sodium_FOUND})
    message("\tSodium Version: ${unofficial-sodium_VERSION}")
endif()

# Set the pthread library file path
set(PTHREAD_LIB "${LIBRARY}/pthreadVC3.lib")

# Include sub-projects.
add_subdirectory("WebShare-Connect")