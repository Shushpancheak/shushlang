cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(PythonInterp REQUIRED)
add_definitions(-D_CRT_SECURE_NO_WARNINGS)

project (
  "shushlang"
)

set (
  SOURCES
  "src/shushlang.cpp"
)

add_executable(${PROJECT_NAME} ${SOURCES})

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
# - - - - - - - - - - - - - PYTHON- - - - - - - - - - - - - - - - - - - - - - -
# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

execute_process(COMMAND python test.py
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
# - - - - - - - - - - - - - GTEST INITIALIZATION- - - - - - - - - - - - - - - -
# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

set(BUILD_TESTS OFF CACHE BOOL "Build tests")
if (BUILD_TESTS)

add_subdirectory(submodules/googletest)
enable_testing()
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
include_directories(${gtest_SOURCE_DIR})

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
# - - - - - - - - - - - - - - - - - TESTS - - - - - - - - - - - - - - - - - - -
# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

set(UNIT_TESTS_NAME "run-tests-${PROJECT_NAME}")
set(UNIT_TESTS_FILE "test/test.cpp")

add_executable(${UNIT_TESTS_NAME} ${UNIT_TESTS_FILE})

# Link test executable against gtest & gtest_main
target_link_libraries(${UNIT_TESTS_NAME} gtest_main ${LIBRARY_NAME})
add_test(${UNIT_TESTS_NAME} ${UNIT_TESTS_NAME})

# For next libraries.
set(BUILD_TESTS OFF CACHE BOOL "Build tests" FORCE)

endif() # BUILD_TESTS

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
# - - - - - - - - - - - - DEPENDENCIES- - - - - - - - - - - - - - - - - - - - -
# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

add_subdirectory(submodules/shush-logs)
add_subdirectory(submodules/shush-dump)
add_subdirectory(submodules/shush-format)
add_subdirectory(submodules/shush-file)

target_link_libraries(shush-logs shush-format)
target_link_libraries(shush-dump shush-format)
target_link_libraries(shush-file shush-dump)

target_link_libraries(${PROJECT_NAME} shush-logs shush-dump shush-file)
include_directories(include inc)
