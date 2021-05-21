cmake_minimum_required(VERSION 3.16)
project(storage_tests)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
# TODO: Can we get rid of protobuf generation here ?
protobuf_generate_cpp(PROTO_SOURCES 
                      PROTO_HEADERS 
                      ${CMAKE_SOURCE_DIR}/storage/network/proto_models/request.proto
                      ${CMAKE_SOURCE_DIR}/storage/network/proto_models/response.proto)

function(add_test test_name)
  add_executable(${test_name} ${test_name}.cpp 
                              ${PROTO_HEADERS}
                              ${PROTO_SOURCES})

# TODO: Actually it's not necessary to link all libraries for every test
# We can improve it by splitting this function up, like add_networking_test() where asio is required
target_link_libraries(${test_name} CONAN_PKG::asio 
                                   CONAN_PKG::gtest
                                   CONAN_PKG::spdlog
                                   CONAN_PKG::protobuf)

target_include_directories(${test_name} PUBLIC ${CONAN_INCLUDE_DIRS_ASIO} 
                                               ${CONAN_INCLUDE_DIRS_GTEST}
                                               ${CONAN_INCLUDE_DIRS_SPDLOG}
                                               ${CMAKE_INCLUDE_DIRS_PROTOBUF}
                                               ${CMAKE_SOURCE_DIR}
                                               ${CMAKE_BINARY_DIR})

set_target_properties(${test_name} PROPERTIES CXX_STANDARD 17
                                              CXX_STANDARD_REQUIRED on
                                              LINKER_LANGUAGE CXX)

endfunction()

add_test(connection_test)
add_test(send_message_test)