cmake_minimum_required(VERSION 3.6)

# LogSpoT
set(LOGSPOT logspot)
if("${PROJECT_SOURCE_DIR}" STREQUAL "")
	set(PROJECT_SOURCE_DIR ".")
endif()
set(LOGSPOT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/${LOGSPOT}/include)
set(LOGSPOT_LIBRARIES ${LOGSPOT})

# Check LogSpoT
if(NOT TARGET ${LOGSPOT})
	add_subdirectory(${PROJECT_SOURCE_DIR}/${LOGSPOT})
endif()

set(DST_TITLE DataSpoT)
project(${DST_TITLE})

set(CMAKE_C_STANDARD   11)
set(CMAKE_CXX_STANDARD 11)

# Version number
set(DST_VERSION_MAJOR 0)
set(DST_VERSION_MINOR 1)
set(DST_NAME dataspot)

# CMake modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake)

# SQLite3
find_package(SQLite3)

# Includes
set(DST_INCLUDE_DIRS
	${PROJECT_SOURCE_DIR}/include
	${SQLITE3_INCLUDE_DIRS}
	${LOGSPOT_INCLUDE_DIR})

# Sources
set(DST_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)
set(SOURCES
	${DST_SOURCE_DIR}/DataSpot.cpp
	${DST_SOURCE_DIR}/Statement.cpp
	${DST_SOURCE_DIR}/Database.cpp)
source_group(Sources FILES ${SOURCES})

# Library
add_library(${DST_NAME} ${SOURCES})
set_target_properties(${DST_NAME} PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(${DST_NAME} ${SQLITE3_LIBRARIES} ${LOGSPOT_LIBRARIES})
target_include_directories(${DST_NAME} PUBLIC ${DST_INCLUDE_DIRS})

# Test
set(DST_TEST_DIR ${PROJECT_SOURCE_DIR}/test)

function(make_test TEST_NAME)
	# Sources
	set(TEST_SOURCES ${DST_TEST_DIR}/${TEST_NAME}.cpp)
	source_group(${TEST_NAME} FILES ${TEST_SOURCES})

	# Executable
	add_executable(${TEST_NAME} ${TEST_SOURCES})
	target_include_directories(${TEST_NAME} PRIVATE ${DST_INCLUDE_DIRS})
	set_target_properties(${TEST_NAME} PROPERTIES LINKER_LANGUAGE CXX)
	target_link_libraries(${TEST_NAME} ${DST_NAME})
	add_test(${TEST_NAME} ${EXECUTABLE_OUTPUT_PATH}/${TEST_NAME})
endfunction(make_test)

make_test(dst-test)

include(CTest)
