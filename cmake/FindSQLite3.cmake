# Copyright (C) 2007-2009 LuaDist.
# Created by Peter Kapec <kapecp@gmail.com>
# Edited by Antonio Caggiano <sir.fahien@gmail.com>
# Redistribution and use of this file is allowed according to the terms of the MIT license.
# For details see the COPYRIGHT file distributed with LuaDist.
#	Note:
#		Searching headers and libraries is very simple and is NOT as powerful as scripts
#		distributed with CMake, because LuaDist defines directories to search for.
#		Everyone is encouraged to contact the author with improvements. Maybe this file
#		becomes part of CMake distribution sometimes.

# - Find sqlite3
# Find the native SQLITE3 headers and libraries.
#
# SQLITE3_INCLUDE_DIRS - where to find sqlite3.h, etc.
# SQLITE3_LIBRARIES    - List of libraries when using sqlite.
# SQLITE3_FOUND        - True if sqlite found.


# Set include and lib hints
if(${CMAKE_SYSTEM_NAME} MATCHES "Android")
	# It is allowed to use a library elsewhere than specified by the toolchain
	set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY BOTH)
	set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE BOTH)
	set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE BOTH)

	set(INCLUDE_HINTS ${CMAKE_CURRENT_SOURCE_DIR}/sqlite3/include)
	set(LIB_HINTS ${CMAKE_CURRENT_SOURCE_DIR}/sqlite3/lib/${CMAKE_ANDROID_ARCH_ABI})
else()
	set(INCLUDE_HINTS /usr/include ${CMAKE_CURRENT_SOURCE_DIR}/sqlite3/include)
	set(LIB_HINTS /usr/lib/x86_64-linux-gnu ${CMAKE_CURRENT_SOURCE_DIR}/sqlite3/lib)
endif()

# Look for the header file.
find_path(SQLITE3_INCLUDE_DIR HINTS ${INCLUDE_HINTS} NAMES sqlite3.h)

# Look for the library.
find_library(SQLITE3_LIBRARY HINTS ${LIB_HINTS} NAMES sqlite3)

# Handle the QUIETLY and REQUIRED arguments and set SQLITE3_FOUND to TRUE if all listed variables are TRUE.
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(SQLITE3 DEFAULT_MSG SQLITE3_LIBRARY SQLITE3_INCLUDE_DIR)

# Copy the results to the output variables.
if(SQLITE3_FOUND)
	set(SQLITE3_LIBRARIES ${SQLITE3_LIBRARY})
	set(SQLITE3_INCLUDE_DIRS ${SQLITE3_INCLUDE_DIR})
else(SQLITE3_FOUND)
	set(SQLITE3_LIBRARIES)
	set(SQLITE3_INCLUDE_DIRS)
endif(SQLITE3_FOUND)

mark_as_advanced(SQLITE3_INCLUDE_DIRS SQLITE3_LIBRARIES)
