CMAKE_MINIMUM_REQUIRED(VERSION 2.6)


PROJECT(MyRayTracer)

file(GLOB CPP "src/*.cpp")
file(GLOB HPP "src/*.hpp")
ADD_EXECUTABLE(MyRayTracer main.cpp ${CPP} ${HPP})
SET(CMAKE_CXX_FLAGS "-std=c++11")