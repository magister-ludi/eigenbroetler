# -*- cmake -*-
# encoding: utf-8

# ########## Project setup ##########
#project(eigenbrötler)
project(eigenbroetler)
cmake_minimum_required(VERSION 2.8)

macro(show arg)
  message(STATUS "${arg}: ${${arg}}")
endmacro(show arg)

include(FindQt4)
include(UseQt4)

set(TOP ${CMAKE_CURRENT_SOURCE_DIR})
set(ARCHIVE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(settings)

set(CPP_INCLUDE_PATH ${TOP}/cpp/include)
include_directories(${CPP_INCLUDE_PATH})

set(CppLibName ${CMAKE_PROJECT_NAME})
set(${CppLibName}_Sources
 ${TOP}/cpp/source/complex_array.cpp
)
set(${CppLibName}_Headers
 ${TOP}/cpp/include/complex_array.h
)

add_library(${CppLibName} SHARED ${${CppLibName}_Sources} ${${CppLibName}_Headers})
set_target_properties(${CppLibName}
  PROPERTIES
  OUTPUT_NAME eigenbrötler
)

set(GuiApp ${CMAKE_PROJECT_NAME}_App)
set(${GuiApp}_Sources
 ${TOP}/cpp/source/main.cpp
 ${TOP}/cpp/source/eigenbrot_window.cpp
)
set(${GuiApp}_Headers
 ${TOP}/cpp/include/eigenbrot_window.h
)

qt4_wrap_cpp(${GuiApp}_Mocfiles ${TOP}/cpp/include/eigenbrot_window.h)
qt4_add_resources(${GuiApp}_Resources ${TOP}/cpp/source/eigenbrot.qrc)
show(${GuiApp}_Sources)
show(${GuiApp}_Headers)
show(${GuiApp}_Mocfiles)

add_definitions(-DQT_SHARED -DQT_WEBKIT -D_REENTRANT)

add_executable(${GuiApp} ${${GuiApp}_Sources} ${${GuiApp}_Headers} ${${GuiApp}_Mocfiles} ${${GuiApp}_Resources})
target_link_libraries(${GuiApp} ${QT_LIBRARIES} ${CppLibName})
set_target_properties(${GuiApp}
  PROPERTIES
  DEBUG_POSTFIX D
  OUTPUT_NAME eigenbrötler
)

if(WithRuby)
  include(ruby_lib)
endif(WithRuby)
