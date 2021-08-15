# -*- cmake -*-
# encoding: utf-8

# ########## Project setup ##########
project(eigenbroetler)
set(CMP0071 NEW)
cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 11)

macro(show arg)
  message(STATUS "===== ${arg}: ${${arg}}")
endmacro(show arg)

set(EIGENBROT_VERSION_MAJOR 0)
set(EIGENBROT_VERSION_MINOR 1)
set(EIGENBROT_VERSION_PATCH 1)
execute_process(COMMAND git log -1 "--format=format:\"%H\"%n" HEAD
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  OUTPUT_VARIABLE EIGENBROT_GIT_ID
  ERROR_STRIP_TRAILING_WHITESPACE)

find_package(Qt5 CONFIG COMPONENTS Core Widgets REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(AUTOUIC_SEARCH_PATHS ${TOP}/ui)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(settings)

find_library(CFITSIO_LIBRARY cfitsio)
find_path (CFITSIO_INCLUDE fitsio.h
           PATH_SUFFIXES cfitsio)
find_library(FFTW3_LIBRARY fftw3)

set(CPP_INCLUDE_PATH ${TOP}/cpp/include)
include_directories(${CPP_INCLUDE_PATH} ${CMAKE_CURRENT_BINARY_DIR}
                    ${CFITSIO_INCLUDE}
)
include_directories(${Qt5Core_INCLUDE_DIRS} ${Qt5Widgets_INCLUDE_DIRS})

###########################################################################
set(flex_input ${TOP}/cpp/source/calculator_lexer.l)
set(flex_output ${CMAKE_CURRENT_BINARY_DIR}/lexer.cpp)
set(bison_input ${TOP}/cpp/source/calculator_parser.y)
set(bison_cpp_output ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp)
set(bison_h_output ${CMAKE_CURRENT_BINARY_DIR}/parser.hpp)
add_custom_command(OUTPUT ${flex_output}
  COMMAND flex -Cf -i -8 -o${flex_output} ${flex_input}
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  DEPENDS ${flex_input}
  )

add_custom_command(OUTPUT ${bison_cpp_output} ${bison_h_output}
  COMMAND bison ${bison_extra_defs} -d -o ${bison_cpp_output} ${bison_input}
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  DEPENDS ${bison_input}
  )

set_source_files_properties(${flex_output}
  PROPERTIES
  COMPILE_FLAGS -Wno-sign-compare
  )
###########################################################################
set(CppLibName ${CMAKE_PROJECT_NAME})
set(${CppLibName}_Sources
  ${bison_cpp_output}
  ${flex_output}
  ${TOP}/cpp/source/calculator.cpp
  ${TOP}/cpp/source/chirpz.cpp
  ${TOP}/cpp/source/complex_array.cpp
  ${TOP}/cpp/source/complex_filters.cpp
  ${TOP}/cpp/source/complex_operations.cpp
  )
set(${CppLibName}_Headers
  ${bison_h_output}
  ${TOP}/cpp/include/calculator.h
  ${TOP}/cpp/include/complex_array.h
  ${TOP}/cpp/include/complex_operations.h
  ${TOP}/cpp/include/display_info.h
  )

add_library(${CppLibName} SHARED ${${CppLibName}_Sources} ${${CppLibName}_Headers})
target_link_libraries(${CppLibName} Qt5::Core ${CFITSIO_LIBRARY} ${FFTW3_LIBRARY})

set_target_properties(${CppLibName}
  PROPERTIES
  OUTPUT_NAME eigenbroetler
  )

set(version_file ${CMAKE_CURRENT_SOURCE_DIR}/cmake/eigenbrot_version.h.in)
set(version_header ${CMAKE_CURRENT_BINARY_DIR}/eigenbrot_version.h)
configure_file(${version_file} ${version_header} @ONLY)

set(GuiApp ${CMAKE_PROJECT_NAME}_App)
set(${GuiApp}_Sources
  ${TOP}/cpp/source/about_dialog.cpp
  ${TOP}/cpp/source/arithmetic.cpp
  ${TOP}/cpp/source/array_window.cpp
  ${TOP}/cpp/source/array_window_2d.cpp
  ${TOP}/cpp/source/array_window_dialogs.cpp
  ${TOP}/cpp/source/chirp_dialog.cpp
  ${TOP}/cpp/source/data_selector.cpp
  ${TOP}/cpp/source/display_info.cpp
  ${TOP}/cpp/source/eigenbroetler_slots.cpp
  ${TOP}/cpp/source/eigenbroetler_window.cpp
  ${TOP}/cpp/source/filter_dialog.cpp
  ${TOP}/cpp/source/formula_dialog.cpp
  ${TOP}/cpp/source/get_images.cpp
  ${TOP}/cpp/source/main.cpp
  ${TOP}/cpp/source/pad_data.cpp
  ${TOP}/cpp/source/phase_ops.cpp
  ${TOP}/cpp/source/plotter.cpp
  )
set(${GuiApp}_Headers
  ${TOP}/cpp/include/complex_array.h
  ${TOP}/cpp/include/data_selector.h
  ${TOP}/cpp/include/plotter.h
  ${TOP}/cpp/include/scaled_plotter.h
  ${version_header}
  )
set(${GuiApp}_MocHeaders
  ${TOP}/cpp/include/about_dialog.h
  ${TOP}/cpp/include/arithmetic.h
  ${TOP}/cpp/include/array_window.h
  ${TOP}/cpp/include/array_window_2d.h
  ${TOP}/cpp/include/array_window_dialogs.h
  ${TOP}/cpp/include/display_info.h
  ${TOP}/cpp/include/chirp_dialog.h
  ${TOP}/cpp/include/eigenbroetler_window.h
  ${TOP}/cpp/include/filter_dialog.h
  ${TOP}/cpp/include/formula_dialog.h
  ${TOP}/cpp/include/get_images.h
  ${TOP}/cpp/include/pad_data.h
  ${TOP}/cpp/include/phase_ops.h
  ${TOP}/cpp/include/plotter.h
  )
set(${GuiApp}_Uifiles
  ${TOP}/cpp/include/about_dialog.ui
  ${TOP}/cpp/include/chirp_dialog.ui
  ${TOP}/cpp/include/close_subwindow_dialog.ui
  ${TOP}/cpp/include/export_components_dialog.ui
  ${TOP}/cpp/include/filter_dialog.ui
  ${TOP}/cpp/include/formula_dialog.ui
  ${TOP}/cpp/include/pad_dialog.ui
  ${TOP}/cpp/include/phase_dialog.ui
  ${TOP}/cpp/include/arithmetic_dialog.ui
  )

set(${GuiApp}_Resources ${TOP}/cpp/source/eigenbrot.qrc)

add_definitions(-DQT_SHARED -D_REENTRANT)

add_executable(${GuiApp}
  ${${GuiApp}_Sources}
  ${${GuiApp}_Headers}
  ${${GuiApp}_MocHeaders}
  ${${GuiApp}_Mocfiles}
  ${${GuiApp}_Uifiles}
  ${${GuiApp}_Resources})
target_link_libraries(${GuiApp} Qt5::Widgets ${CppLibName})

set_target_properties(${GuiApp}
  PROPERTIES
  OUTPUT_NAME eigenbroetler
  )
