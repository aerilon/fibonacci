# CMakeLists.txt in src
cmake_minimum_required(VERSION 3.0)

include(CheckCXXCompilerFlag)

project(fibo)
set(app_SOURCES
        src/api.cc
        src/app.cc
        src/app_client.cc
        src/app_server.cc
        src/worker.cc
	)

add_executable(app ${app_SOURCES})

CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
if(COMPILER_SUPPORTS_CXX11)
	set_property(TARGET app APPEND_STRING PROPERTY COMPILE_FLAGS "-std=c++11 ")
else()
	message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

set_property(TARGET app APPEND_STRING PROPERTY COMPILE_FLAGS "-g -O2 ")

set_property(TARGET app APPEND_STRING PROPERTY LINK_FLAGS "-lboost_system ")
set_property(TARGET app APPEND_STRING PROPERTY LINK_FLAGS "-lpthread ")
