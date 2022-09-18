cmake_minimum_required(VERSION 2.6)
project(monitor)

find_package(Curses REQUIRED)
include_directories(include ${CURSES_INCLUDE_DIR} /opt/ros/noetic/include /opt/ros/noetic/lib)

file(GLOB SOURCES "src/*.cpp")

add_executable(monitor ${SOURCES})

set_property(TARGET monitor PROPERTY CXX_STANDARD 17)

target_link_libraries(monitor 
${CURSES_LIBRARIES} 
pthread
-I/opt/ros/noetic/include -L/opt/ros/noetic/lib
-lroscpp -lrostime -lrosconsole -lroscpp_serialization)

target_compile_options(monitor PRIVATE -g -pthread -Wall -Wextra)