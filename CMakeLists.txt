# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

# Set the vcpkg toolchain file if vcpkg is being used
if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "-DCMAKE_TOOLCHAIN_FILE=R:/vcpkg/scripts/buildsystems/vcpkg.cmake"
      CACHE STRING "")
endif()

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project (WebShare-Connect C)

# Find packages
find_package(czmq CONFIG REQUIRED)
find_package(ZeroMQ REQUIRED)

# Include sub-projects.
add_subdirectory ("WebShare-Connect")

# Link libraries to your project (replace 'your_target_name' with the actual target name)
# target_link_libraries(your_target_name PRIVATE czmq ZeroMQ::ZeroMQ)
