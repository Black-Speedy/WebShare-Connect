cmake_minimum_required(VERSION 3.16)

if(WIN32)
            message("VCPKG_ROOT: $ENV{VCPKG_ROOT}")

# Set the vcpkg toolchain file if vcpkg is being used
    if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
        # if(NOT DEFINED VCPKG_ROOT)
            # Checks if vcpkg is installed and added to the PATH then makes a 
            execute_process(
            COMMAND where vcpkg
            OUTPUT_VARIABLE VCPKG_EXE_PATH
            OUTPUT_STRIP_TRAILING_WHITESPACE
            )
        
            get_filename_component(VCPKG_ROOT "${VCPKG_EXE_PATH}" DIRECTORY cache string "")
            set(ENV{VCPKG_ROOT} "${VCPKG_ROOT}")
            message("VCPKG_ROOT: $ENV{VCPKG_ROOT}")
        # endif()

        set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
        set(ENV{CMAKE_TOOLCHAIN_FILE} "${CMAKE_TOOLCHAIN_FILE}")
    endif()
endif()

# Enable Hot Reload for MSVC compilers if supported.
if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
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


# Include sub-projects #

# Compile Everything
# add_subdirectory("WebShare-Connect/APP")

# Compile Core Libraries
add_subdirectory("WebShare-Connect/Core")

# Compile CLI
add_subdirectory("WebShare-Connect/CLI")
# Compile Core GUI
add_subdirectory("WebShare-Connect/GUI")

# Link libraries to your project (replace 'your_target_name' with the actual target name)
# target_link_libraries(your_target_name PRIVATE czmq ZeroMQ::ZeroMQ)
