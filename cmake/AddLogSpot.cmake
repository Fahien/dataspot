# LogSpoT
set(LOGSPOT logspot)
set(LOGSPOT_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${LOGSPOT}/include)
if(ANDROID)
	set(LOGSPOT_LIBRARY ${LOGSPOT} log)
else()
	set(LOGSPOT_LIBRARY ${LOGSPOT})
endif()

set(LOGSPOT_INCLUDE_DIRS ${LOGSPOT_INCLUDE_DIR})
set(LOGSPOT_LIBRARIES ${LOGSPOT_LIBRARY})

# Check LogSpoT
if(NOT TARGET ${LOGSPOT})
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/${LOGSPOT})
endif()