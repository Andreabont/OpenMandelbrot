cmake_minimum_required(VERSION 2.6)
project(mandelbrot)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O2 -ffast-math -fopenmp")
set(EXECUTABLE_NAME "mandelbrot")

add_executable(${EXECUTABLE_NAME} main.cpp functions.hpp Fractal.hpp Fractal.cpp)

link_directories(/usr/local/lib)

set(BOOST_LIBS program_options)
find_package(Boost COMPONENTS ${BOOST_LIBS} REQUIRED)

target_link_libraries (${EXECUTABLE_NAME} ${Boost_LIBRARIES})
target_link_libraries (${EXECUTABLE_NAME} sfml-window)
target_link_libraries (${EXECUTABLE_NAME} sfml-system)
target_link_libraries (${EXECUTABLE_NAME} sfml-graphics)

install(TARGETS ${EXECUTABLE_NAME} RUNTIME DESTINATION bin)
