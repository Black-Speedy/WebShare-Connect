cmake_minimum_required(VERSION 3.16)

# Enable testing
enable_testing()

# Find required packages for testing
find_package(Threads REQUIRED)

# Include directories for accessing core headers
include_directories(${CMAKE_SOURCE_DIR}/core)

# Test for job queue implementation
add_executable(test_job_queue test_job_queue.c)
target_link_libraries(test_job_queue 
    PRIVATE 
    core
    Threads::Threads
)

add_executable(test_job_queue_comprehensive test_job_queue_comprehensive.c)
target_link_libraries(test_job_queue_comprehensive PRIVATE core Threads::Threads)


# Add the test to CTest
add_test(NAME JobQueueTest COMMAND test_job_queue)
add_test(NAME JobQueueComprehensiveTest COMMAND test_job_queue_comprehensive)

# Set test properties
set_tests_properties(JobQueueTest PROPERTIES
    TIMEOUT 30
    PASS_REGULAR_EXPRESSION "test_job_queue passed"
)

set_tests_properties(JobQueueComprehensiveTest PROPERTIES
    TIMEOUT 60
    PASS_REGULAR_EXPRESSION "ALL TESTS PASSED"
)

# Optional: Add more tests as you create them
# add_executable(test_cthreads test_cthreads.c)
# target_link_libraries(test_cthreads PRIVATE core Threads::Threads)
# add_test(NAME CThreadsTest COMMAND test_cthreads)

# add_executable(test_tasks test_tasks.c)
# target_link_libraries(test_tasks PRIVATE core)
# add_test(NAME TasksTest COMMAND test_tasks)

# Set common compile options for all tests
if(MSVC)
    target_compile_options(test_job_queue PRIVATE /W4)
else()
    target_compile_options(test_job_queue PRIVATE -Wall -Wextra -pedantic)
endif()

# Add custom target to run all tests
add_custom_target(run_tests
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    DEPENDS test_job_queue
    COMMENT "Running all tests"
)