# -*- cmake -*-
# encoding: utf-8

set(TOP ${CMAKE_CURRENT_SOURCE_DIR})

set(ARCHIVE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})
set(EXECUTABLE_OUTPUT_PATH ${TOP})
set(EXTERNALS_DIR ${TOP}/external_dependencies)
set(INCLUDE_DIR ${EXTERNALS_DIR}/include)

if(UNIX)
  set(CMAKE_GENERATOR "Unix Makefiles")
  set(COMMON_FLAGS "-ansi -DSTDC_HEADERS -Wall -Werror -W -Wshadow -Wformat -Wpointer-arith -Wcast-align -Wwrite-strings")
  set(EXTRA_C_FLAGS "-Wnested-externs -Wmissing-prototypes -Wstrict-prototypes")
  set(EXTRA_CXX_FLAGS "-Woverloaded-virtual")
endif(UNIX)

if(MSVC)
  # ???
endif(MSVC)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  add_definitions("-DPTR_64_BIT")
else(CMAKE_SIZEOF_VOID_P EQUAL 8)
endif(CMAKE_SIZEOF_VOID_P EQUAL 8)

set(COMPILE_DEFINITIONS_DEBUG "${COMPILE_DEFINITIONS_DEBUG} -DDEBUG")

set(BASE_C_FLAGS ${CMAKE_C_FLAGS})
set(BASE_CXX_FLAGS ${CMAKE_CXX_FLAGS})

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${COMMON_FLAGS} ${EXTRA_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMMON_FLAGS} ${EXTRA_CXX_FLAGS}")

# vim: filetype=cmake
