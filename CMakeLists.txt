cmake_minimum_required(VERSION 3.28.3)
project(MySQLConnectionPool)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -O0 -ggdb -std=c++14 -Wall -Wno-deprecated -Wno-unused-function -Wno-builtin-macro-redefined")

include_directories(.)
include_directories(/usr/include)

link_directories(/usr/lib/x86_64-linux-gnu)

set(LIB_SRC 
	src/MySQLConn.cc
)

add_library(MySQLConnectionPool SHARED ${LIB_SRC})

set(LIB_LIB
    MySQLConnectionPool
 	libmysqlclient
)

# 测试聊天客户端模块
# add_executable(test_client test/test_client.cc)
# add_dependencies(test_client MySQLConnectionPool)
# target_link_libraries(test_client ${LIB_LIB})

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

