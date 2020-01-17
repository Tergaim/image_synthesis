CMAKE_MINIMUM_REQUIRED(VERSION 2.6)


PROJECT(MyRayTracer)

file(GLOB CPP "./*.cpp")
file(GLOB HPP "./*.hpp")
ADD_EXECUTABLE(MyRayTracer ${CPP} ${HPP} )