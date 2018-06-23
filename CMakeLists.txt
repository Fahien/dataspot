cmake_minimum_required(VERSION 3.6)

set(CMAKE_C_STANDARD   11)
set(CMAKE_CXX_STANDARD 11)

set(DST_TITLE DataSpot)
project(${DST_TITLE})

# Version number
set(DST_VERSION_MAJOR 0)
set(DST_VERSION_MINOR 2)
set(DST_NAME dataspot)

# CMake modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# SQLite3
find_package(SQLite3)
include(AddLogSpot)

# Includes
set(DST_INCLUDE_DIRS
	${PROJECT_SOURCE_DIR}/include
	${SQLITE3_INCLUDE_DIRS}
	${LOGSPOT_INCLUDE_DIRS})

# Sources
set(DST_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)
set(SOURCES
	${DST_SOURCE_DIR}/DataSpot.cpp
	${DST_SOURCE_DIR}/Statement.cpp
	${DST_SOURCE_DIR}/Database.cpp)
source_group(Sources FILES ${SOURCES})

# Library
add_library(${DST_NAME} ${SOURCES})
target_include_directories(${DST_NAME} PUBLIC ${DST_INCLUDE_DIRS})
target_link_libraries(${DST_NAME} ${SQLITE3_LIBRARIES} ${LOGSPOT_LIBRARIES})

# Test
set(DST_TEST_DIR ${PROJECT_SOURCE_DIR}/test)

function(make_test TEST_NAME)
	# Sources
	set(TEST_SOURCES ${DST_TEST_DIR}/${TEST_NAME}.cpp)
	source_group(${TEST_NAME} FILES ${TEST_SOURCES})

	# Executable
	add_executable(${TEST_NAME} ${TEST_SOURCES})
	target_include_directories(${TEST_NAME} PRIVATE ${DST_INCLUDE_DIRS})
	target_link_libraries(${TEST_NAME} ${DST_NAME})
	add_test(${TEST_NAME} ${EXECUTABLE_OUTPUT_PATH}/${TEST_NAME})
endfunction(make_test)

make_test(dst-test)

include(CTest)
