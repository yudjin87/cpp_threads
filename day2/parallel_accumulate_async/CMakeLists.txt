project(parallel_accumulate_async)
cmake_minimum_required(VERSION 2.8)
aux_source_directory(. SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})
add_definitions("-std=c++14 -O2 -flto")
target_link_libraries(${PROJECT_NAME} pthread)
