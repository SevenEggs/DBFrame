# sudo apt-get install libmysqlclient-dev
# mysql.h 头文件以及动态库和静态库

cmake_minimum_required(VERSION 3.22)

project(dbframe)

#CMAKE_SOURCE_DIR当前目录
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/")

# set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(fmt)

add_subdirectory(dbimpl)

add_subdirectory(example/sync)

add_subdirectory(example/async)
