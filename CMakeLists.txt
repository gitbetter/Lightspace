cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 14)

set(PROJECT_NAME Lightspace)

project(${PROJECT_NAME} VERSION 1.0.0 LANGUAGES CXX)

option(DEVELOPMENT "Generate a development build" OFF)
option(DOUBLE_PRECISION "Double precision floating point numbers" OFF)

include(${CMAKE_SOURCE_DIR}/tests/CMakeLists.txt)
include(${CMAKE_SOURCE_DIR}/source/CMakeLists.txt)
include(${CMAKE_SOURCE_DIR}/thirdparty/CMakeLists.txt)

set(SOURCES ${CORE_SOURCES} ${THIRD_PARTY_SOURCES})

set(INCLUDES ${CORE_INCLUDES} ${THIRD_PARTY_INCLUDES})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)

if(DEVELOPMENT)
	add_compile_definitions(DEVELOPMENT)
endif(DEVELOPMENT)

if(DOUBLE_PRECISION)
	add_compile_definitions(DOUBLE_PRECISION)
endif(DOUBLE_PRECISION)

# Set up Lightspace library 
add_library(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDES})

# Set up Lightspace demo executable
add_executable(${PROJECT_NAME}_Demo ${CORE_DIR}/main.cpp)
target_link_libraries(${PROJECT_NAME}_Demo ${PROJECT_NAME})

# Set up Lightspace tests executable
add_executable(Tests ${TEST_SOURCES})
target_link_libraries(Tests ${PROJECT_NAME})

foreach(FILE ${SOURCES}) 
	get_filename_component(PARENT_DIR "${FILE}" DIRECTORY)
	string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "" GROUP "${PARENT_DIR}")
	string(REPLACE "/" "\\" GROUP "${GROUP}")
	source_group("${GROUP}" FILES "${FILE}")
endforeach()