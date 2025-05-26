cmake_minimum_required(VERSION 3.28.3)
project(MySQLConnectionPool)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -O0 -ggdb -std=c++14 -Wall -Wno-deprecated -Wno-unused-function -Wno-builtin-macro-redefined")

include_directories(.)
include_directories(/usr/include)
include_directories(/usr/local/include)

link_directories(/usr/lib/x86_64-linux-gnu)
link_directories(/usr/local/lib)

set(LIB_SRC 
	src/MySQLConn.cc
	src/ConnectionPool.cc
)

add_library(MySQLConnectionPool SHARED ${LIB_SRC})

set(LIB_LIB
    MySQLConnectionPool
 	mysqlclient
	jsoncpp
)

# 测试数据库操作
add_executable(test test/test.cc)
add_dependencies(test MySQLConnectionPool)
target_link_libraries(test ${LIB_LIB})

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

