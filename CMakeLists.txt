cmake_minimum_required(VERSION 3.12.0 FATAL_ERROR)

project(express VERSION 0.1.0 LANGUAGES CXX)

option(EXPRESS_BUILD_TESTING "Build test program" ON)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

set(EXPRESS_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/include/express)

set(
	EXPRESS_HEADERS
	${EXPRESS_INCLUDE_DIR}/Showable.hpp
	${EXPRESS_INCLUDE_DIR}/Overload.hpp
	${EXPRESS_INCLUDE_DIR}/Variable.hpp
	${EXPRESS_INCLUDE_DIR}/Expr.hpp
	${EXPRESS_INCLUDE_DIR}/AInt.hpp
)

add_subdirectory(deps/fmt-6.0.0)
add_subdirectory(src)

if(EXPRESS_BUILD_TESTING)
add_subdirectory(testing)
endif()
