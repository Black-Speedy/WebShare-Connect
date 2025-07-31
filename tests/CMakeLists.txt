cmake_minimum_required(VERSION 4.0.2)

include(CTest)      
include_directories(${CMAKE_SOURCE_DIR}/core)
include_directories(${CMAKE_BINARY_DIR}/core)
include_directories(${CMAKE_SOURCE_DIR}/wsc)
include_directories(${CMAKE_SOURCE_DIR}/gui)
find_package(Threads REQUIRED)

#-----------------------------------------------------------------------------#
# 1. Global flags for coverage-enabled Debug builds
#-----------------------------------------------------------------------------#
set(CMAKE_BUILD_TYPE "Debug")
set(COVERAGE_FLAGS "-fprofile-instr-generate -fcoverage-mapping")
add_compile_options(${COVERAGE_FLAGS})
add_link_options(${COVERAGE_FLAGS})

#-----------------------------------------------------------------------------#
# 2. Tell CMake which test targets exist and which sources each target needs
#-----------------------------------------------------------------------------#
set(TESTS
  test_job_queue
  test_job_queue_comprehensive
  test_cli_options
)

set(test_job_queue_SOURCES
    test_job_queue.c)

set(test_job_queue_comprehensive_SOURCES
    test_job_queue_comprehensive.c)

set(test_cli_options_SOURCES
    test_cli_options.c
    ../wsc/options.c
    ../core/variables.c)

#-----------------------------------------------------------------------------#
# 3. Generate every test target, attach libraries, and register with CTest
#-----------------------------------------------------------------------------#
set(COVERAGE_OBJECTS)       # holds $<TARGET_FILE:...> for llvm-cov

foreach(test IN LISTS TESTS)
  add_executable(${test} ${${test}_SOURCES})
  target_link_libraries(${test} PRIVATE core Threads::Threads)
  list(APPEND COVERAGE_OBJECTS --object=$<TARGET_FILE:${test}>)
  add_test(NAME ${test} COMMAND ${test})
endforeach()

# Optional: per-test expectations
set_tests_properties(test_job_queue
  PROPERTIES TIMEOUT 30 PASS_REGULAR_EXPRESSION "test_job_queue passed")
set_tests_properties(test_job_queue_comprehensive
  PROPERTIES TIMEOUT 60 PASS_REGULAR_EXPRESSION "ALL TESTS PASSED")
set_tests_properties(test_cli_options
  PROPERTIES TIMEOUT 30 PASS_REGULAR_EXPRESSION "All CLI options tests passed")

#-----------------------------------------------------------------------------#
# 4. Unified targets for “run all” and for HTML coverage
#-----------------------------------------------------------------------------#
add_custom_target(run_tests
  COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
  DEPENDS ${TESTS}
  COMMENT "Running all tests")

find_program(LLVM_COV_EXECUTABLE   NAMES llvm-cov   REQUIRED)
find_program(LLVM_PROFDATA_EXECUTABLE NAMES llvm-profdata REQUIRED)

set(PROFILE_DATA        ${CMAKE_BINARY_DIR}/coverage.profdata)
set(COVERAGE_HTML_DIR   ${CMAKE_BINARY_DIR}/coverage-html)

if(WIN32)
  set(REMOVE_PROFRAW_CMD cmd /C "if exist coverage_*.profraw del /Q coverage_*.profraw")
else()
  set(REMOVE_PROFRAW_CMD sh -c "ls coverage_*.profraw 1>/dev/null 2>&1 && rm -f coverage_*.profraw || true")
endif()


add_custom_target(coverage
  COMMENT "Executing full test suite and building unified coverage report..."
  COMMAND ${REMOVE_PROFRAW_CMD}
  COMMAND ${CMAKE_COMMAND} -E env
          LLVM_PROFILE_FILE=coverage_%p_%m.profraw
          ${CMAKE_CTEST_COMMAND} --output-on-failure
  COMMAND ${LLVM_PROFDATA_EXECUTABLE} merge -sparse coverage_*.profraw
          -o ${PROFILE_DATA}
  COMMAND ${LLVM_COV_EXECUTABLE} show
          --format=html
          --output-dir=${COVERAGE_HTML_DIR}
          --instr-profile=${PROFILE_DATA}
          ${COVERAGE_OBJECTS}
          -show-line-counts-or-regions
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  VERBATIM)

