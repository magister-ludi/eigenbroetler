# -*- cmake -*-
# encoding: utf-8

# ########## Project setup ##########
#project(eigenbrötler)
project(eigenbroetler)
cmake_minimum_required(VERSION 2.8)

macro(show arg)
  message(STATUS "===== ${arg}: ${${arg}}")
endmacro(show arg)

include(FindQt4)
include(UseQt4)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(settings)
include(cfitsio_lib)

set(CPP_INCLUDE_PATH ${TOP}/cpp/include)
include_directories(${CPP_INCLUDE_PATH} ${INCLUDE_DIR})

set(CppLibName ${CMAKE_PROJECT_NAME})
set(${CppLibName}_Sources
 ${TOP}/cpp/source/complex_array.cpp
)
set(${CppLibName}_Headers
 ${TOP}/cpp/include/complex_array.h
)

add_library(${CppLibName} SHARED ${${CppLibName}_Sources} ${${CppLibName}_Headers})
target_link_libraries(${CppLibName} ${QT_LIBRARIES} ${cfitsio_LIBRARIES})
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

add_definitions(-DQT_SHARED -DQT_WEBKIT -D_REENTRANT)

add_executable(${GuiApp} ${${GuiApp}_Sources} ${${GuiApp}_Headers} ${${GuiApp}_Mocfiles} ${${GuiApp}_Resources})
target_link_libraries(${GuiApp} ${CppLibName})

set_target_properties(${GuiApp}
  PROPERTIES
  DEBUG_POSTFIX D
  OUTPUT_NAME eigenbrötler
)

if(WithRuby)
  include(ruby_lib)
endif(WithRuby)
