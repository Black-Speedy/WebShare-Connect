cmake_minimum_required(VERSION 3.16)

# project(wsc LANGUAGES C)


add_executable(wsc main.c
               handlearguments.c
               options.c
               ../core/variables.c

)

# Detect Clang with MSVC-like frontend
if (CMAKE_C_COMPILER_ID STREQUAL "Clang" AND MSVC)
  message(STATUS "Using Clang-cl with MSVC ABI for C")

  target_compile_options(wsc PRIVATE
    /W4
    # /WX
    /RTC1
    /RTCs
    /permissive-             # Enforce standard conformance
    /Zc:__cplusplus          # Proper __cplusplus macro
    /Zc:inline               # Standard inline semantics
    /Zc:preprocessor         # Conformant macro handling
    /utf-8                   # UTF-8 encoding for source files
    /std:c17                 # Explicitly set C standard
    /GS                      # Buffer security checks
    /sdl                     # Enable additional security checks

    /w14242                  # Variable used but never initialized
    /w14388                  # Signed/unsigned mismatch
    /w14549                  # Invalid extension used
    /w14905 /w14906          # Wide string literal cast warnings
  )
endif()

if (CMAKE_C_COMPILER_ID STREQUAL "Clang" AND NOT MSVC)
  message(STATUS "Using native Clang (Unix-style CLI)")

  target_compile_options(wsc PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -Wshadow
    -Wstrict-prototypes
    -Wmissing-prototypes
    -Wconversion
    -Wdouble-promotion
    -Wfloat-equal
    -Wcast-align
    -Wundef
    -O2
    -march=native
    -std=c17
    -fstrict-aliasing
    -fvisibility=hidden
    -fno-omit-frame-pointer
  )
endif()


set_target_properties(wsc PROPERTIES
  C_STANDARD 11
  C_STANDARD_REQUIRED ON
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