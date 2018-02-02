cmake_minimum_required(VERSION 3.6)
set(DST_TITLE DataSpoT)
project(${DST_TITLE})

set(CMAKE_C_STANDARD   11)
set(CMAKE_CXX_STANDARD 11)

# Version number
set(DST_VERSION_MAJOR 0)
set(DST_VERSION_MINOR 1)
set(DST_NAME dataspot)

# Includes
set(DST_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
include_directories(${DST_INCLUDE_DIR})

# Sources
set(DST_SOURCE_DIR ${PROJECT_SOURCE_DIR}/source)
set(SOURCES
	${DST_SOURCE_DIR}/DataSpot.cpp
	${DST_SOURCE_DIR}/Statement.cpp
	${DST_SOURCE_DIR}/Database.cpp
	${DST_SOURCE_DIR}/sqlite3.c)
source_group(Sources FILES ${SOURCES})

# Library
add_library(${DST_NAME} ${SOURCES})
set_target_properties(${DST_NAME} PROPERTIES LINKER_LANGUAGE CXX)

# Test
set(DST_TEST_DIR ${PROJECT_SOURCE_DIR}/test)

function(make_test TEST_NAME)
	# Sources
	set(TEST_SOURCES ${DST_TEST_DIR}/${TEST_NAME}.cpp)
	source_group(${TEST_NAME} FILES ${TEST_SOURCES})

	# Executable
	add_executable(${TEST_NAME} ${TEST_SOURCES})
	set_target_properties(${TEST_NAME} PROPERTIES LINKER_LANGUAGE CXX)
	target_link_libraries(${TEST_NAME} ${DST_NAME})
	add_test(${TEST_NAME} ${EXECUTABLE_OUTPUT_PATH}/${TEST_NAME})
endfunction(make_test)

make_test(simple-test)

include(CTest)
