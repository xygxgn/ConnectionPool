cmake_minimum_required(VERSION 3.0.2)
project(connectionPool)

add_compile_options(-std=c++11)

set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_LIST_DIR}/lib)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_LIST_DIR}/bin)

include_directories(
  include
)

aux_source_directory(./src SRC_DIR)

add_library(connectionPool
  ${SRC_DIR}
)

target_link_libraries(connectionPool
  mysqlclient
  pthread
)

add_executable(test
  main.cpp
)

target_link_libraries(test
  connectionPool
)