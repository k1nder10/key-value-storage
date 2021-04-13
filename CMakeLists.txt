cmake_minimum_required(VERSION 3.16)

project(key_value_storage LANGUAGES CXX)

# Add conan package manager
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

# Manage storage compilation
add_executable(storage storage/main.cpp
                       storage/network/connection.hpp
                       storage/network/connection.cpp
                       storage/network/server.hpp
                       storage/network/server.cpp
                       storage/network/operation_type.hpp
                       storage/utils/noncopyable.hpp)

target_link_libraries(storage CONAN_PKG::asio 
                              CONAN_PKG::spdlog)
target_include_directories(storage PUBLIC ${CONAN_INCLUDE_DIRS_ASIO}
                                          ${CONAN_INCLUDE_DIRS_SPDLOG}
                                          ${CMAKE_SOURCE_DIR})

set_target_properties(storage PROPERTIES CXX_STANDARD 17
                                         CXX_STANDARD_REQUIRED on
                                         LINKER_LANGUAGE CXX)

# Export compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Manage build type
set(DEFAULT_BUILD_TYPE "Debug")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message("Message: Setting build type to '${DEFAULT_BUILD_TYPE}' as none was specified.")
    set(CMAKE_BUILD_TYPE "${DEFAULT_BUILD_TYPE}" CACHE STRING "Choose the type of build." FORCE)
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(storage PRIVATE $<$<CONFIG:RELEASE>:-O2>)
    target_compile_options(storage PRIVATE $<$<CONFIG:DEBUG>:-Wall;-Wextra;-Wpedantic>)
endif()

# Manage tests compilation
option(COMPILE_STORAGE_TESTS "Compile tests for the storage application" ON)
if (COMPILE_STORAGE_TESTS)
    add_subdirectory(tests)
endif()