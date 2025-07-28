cmake_minimum_required(VERSION 4.0.2)
project(core LANGUAGES C)

add_library(core
    core.c
    core.h
    cthreads.c
    cthreads.h
    jobqueue.c
    jobqueue.h
)

# Detect Clang with MSVC-like frontend
if (CMAKE_C_COMPILER_ID STREQUAL "Clang" AND MSVC)
  message(STATUS "Using Clang-cl with MSVC ABI for C")

  target_compile_options(core PRIVATE
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

  target_compile_options(core PRIVATE
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


set_target_properties(core PROPERTIES
  C_STANDARD 11
  C_STANDARD_REQUIRED ON
)

target_link_libraries(core PRIVATE libzmq)

target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

include(GNUInstallDirs)
install(TARGETS core
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
