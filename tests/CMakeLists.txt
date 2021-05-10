cmake_minimum_required(VERSION 3.16)
project(storage_tests)

# Add conan package manager
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
protobuf_generate_cpp(PROTO_SOURCES 
                      PROTO_HEADERS 
                      ${CMAKE_SOURCE_DIR}/storage/network/proto_models/command.proto)

# Manage tests compilation
add_executable(connection_test connection_test.cpp 
                               ${PROTO_HEADERS}
                               ${PROTO_SOURCES})

target_link_libraries(connection_test CONAN_PKG::asio 
                                      CONAN_PKG::gtest
                                      CONAN_PKG::spdlog
                                      CONAN_PKG::protobuf)
target_include_directories(connection_test PUBLIC ${CONAN_INCLUDE_DIRS_ASIO} 
                                                  ${CONAN_INCLUDE_DIRS_GTEST}
                                                  ${CONAN_INCLUDE_DIRS_SPDLOG}
                                                  ${CMAKE_INCLUDE_DIRS_PROTOBUF}
                                                  ${CMAKE_SOURCE_DIR}
                                                  ${CMAKE_BINARY_DIR})

set_target_properties(connection_test PROPERTIES CXX_STANDARD 17
                                                 CXX_STANDARD_REQUIRED on
                                                 LINKER_LANGUAGE CXX)