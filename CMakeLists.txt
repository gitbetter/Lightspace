cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 14)

project(Lightspace VERSION 1.0.0 LANGUAGES CXX)

option(DEVELOPMENT "Generate a development build" OFF)
option(DOUBLE_PRECISION "Double precision floating point numbers" OFF)

set(PROJECT_NAME Lightspace)

include(${CMAKE_SOURCE_DIR}/tests/CMakeLists.txt)
include(${CMAKE_SOURCE_DIR}/source/CMakeLists.txt)
include(${CMAKE_SOURCE_DIR}/thirdparty/CMakeLists.txt)

set(SOURCES ${TEST_SOURCES} ${THIRD_PARTY_SOURCES} ${CORE_SOURCES})

set(INCLUDES ${CORE_INCLUDES} ${THIRD_PARTY_INCLUDES})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)

if(DEVELOPMENT)
	add_compile_definitions(DEVELOPMENT)
endif(DEVELOPMENT)

if(DOUBLE_PRECISION)
	add_compile_definitions(DOUBLE_PRECISION)
endif(DOUBLE_PRECISION)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDES})

foreach(FILE ${SOURCES}) 
	get_filename_component(PARENT_DIR "${FILE}" DIRECTORY)
	string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "" GROUP "${PARENT_DIR}")
	string(REPLACE "/" "\\" GROUP "${GROUP}")
	source_group("${GROUP}" FILES "${FILE}")
endforeach()