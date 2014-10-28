
cmake_minimum_required(VERSION 2.8)
project(lazy-init)

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(
  ./include
)

set(HEADERS
	./include/lazy-init/lazy-init.hpp
)

add_executable(lazy-init
	${HEADERS}
	main.cpp
)

install(TARGETS lazy-init RUNTIME DESTINATION bin)
