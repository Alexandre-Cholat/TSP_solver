cmake_minimum_required(VERSION 3.14)
project(TSPsolver)

# --- Global options ---
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable testing target build
set(BUILD_TESTS ON)


# Ensure executables build directly into build/
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# --- Debug/Release configuration ---
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# --- Compiler flags for project sources ---
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# --- Directories ---
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/cpp/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/cpp/include)
set(TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tests)

# --- Find Headers ---
file(GLOB_RECURSE HEADERS "${INCLUDE_DIR}/*.hpp")

# --- Main Executable ---
add_executable(TSPsolver ${SOURCE_DIR}/main.cpp ${HEADERS})
target_include_directories(TSPsolver PRIVATE ${INCLUDE_DIR})

# --- Tests Executable with GoogleTest ---
enable_testing()
file(GLOB TEST_SOURCES "${TEST_DIR}/*.cpp")

if(TEST_SOURCES)
  include(FetchContent)
  
  # Fetch latest GoogleTest
  FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.14.0
    SYSTEM # Treat GoogleTest headers as system headers to suppress external warnings
  )
  
  set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(googletest)

  # Disable -Werror specifically for GoogleTest targets
  if(TARGET gtest)
    target_compile_options(gtest PRIVATE -w)
  endif()
  if(TARGET gtest_main)
    target_compile_options(gtest_main PRIVATE -w)
  endif()

  add_executable(tests ${TEST_SOURCES} ${HEADERS})
  target_include_directories(tests PRIVATE ${INCLUDE_DIR})
  target_link_libraries(tests PRIVATE gtest_main)
  add_test(NAME tests COMMAND tests)
endif()