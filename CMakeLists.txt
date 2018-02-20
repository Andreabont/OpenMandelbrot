cmake_minimum_required(VERSION 3.0)
project(mandelbrot)

set(EXECUTABLE_NAME "mandelbrot")
set(CMAKE_CXX_STANDARD 11)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif(NOT CMAKE_BUILD_TYPE)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -ffast-math -fopenmp")

add_executable(${EXECUTABLE_NAME} main.cpp functions.hpp Fractal.hpp Fractal.cpp)

#Find Boost
set(BOOST_LIBS program_options)
find_package(Boost COMPONENTS ${BOOST_LIBS} REQUIRED)
target_link_libraries (${EXECUTABLE_NAME} ${Boost_LIBRARIES})

# Find SFML
set(SFML_LIBS sfml-window sfml-system sfml-graphics)
target_link_libraries (${EXECUTABLE_NAME} ${SFML_LIBS})

install(TARGETS ${EXECUTABLE_NAME} RUNTIME DESTINATION bin)
