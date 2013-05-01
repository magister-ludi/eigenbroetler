# -*- cmake -*-
# encoding: utf-8

# Qt open source at http://qt-project.org/
# cd ~/arbeitsplatz/projects/eigenbroetler && make debug >/dev/null && eigenbroetlerD
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

find_library(CFITSIO_LIBRARY cfitsio)
find_library(FFTW3_LIBRARY fftw3)

set(CPP_INCLUDE_PATH ${TOP}/cpp/include)
include_directories(${CPP_INCLUDE_PATH} ${CMAKE_CURRENT_BINARY_DIR})
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
 ${TOP}/cpp/source/complex_array.cpp
)
set(${CppLibName}_Headers
 ${bison_h_output}
 ${TOP}/cpp/include/calculator.h
 ${TOP}/cpp/include/complex_array.h
 ${TOP}/cpp/include/display_info.h
)

add_library(${CppLibName} SHARED ${${CppLibName}_Sources} ${${CppLibName}_Headers})
target_link_libraries(${CppLibName} ${QT_LIBRARIES} ${CFITSIO_LIBRARY} ${FFTW3_LIBRARY})
set_target_properties(${CppLibName}
  PROPERTIES
  OUTPUT_NAME eigenbroetler
)

set(GuiApp ${CMAKE_PROJECT_NAME}_App)
set(${GuiApp}_Sources
  ${TOP}/cpp/source/array_window.cpp
  ${TOP}/cpp/source/array_window_2d.cpp
  ${TOP}/cpp/source/display_info.cpp
  ${TOP}/cpp/source/eigenbrot_window.cpp
  ${TOP}/cpp/source/formula_dialog.cpp
  ${TOP}/cpp/source/main.cpp
  ${TOP}/cpp/source/plotter.cpp
  #${TOP}/cpp/source/scaled_plotter.cpp
  )
set(${GuiApp}_Headers
  ${TOP}/cpp/include/array_window.h
  ${TOP}/cpp/include/array_window_2d.h
  ${TOP}/cpp/include/complex_array.h
  ${TOP}/cpp/include/display_info.h
  ${TOP}/cpp/include/eigenbrot_window.h
  ${TOP}/cpp/include/formula_dialog.h
  ${TOP}/cpp/include/plotter.h
  ${TOP}/cpp/include/scaled_plotter.h
  )

qt4_wrap_cpp(${GuiApp}_Mocfiles
  ${TOP}/cpp/include/array_window.h
  ${TOP}/cpp/include/array_window_2d.h
  ${TOP}/cpp/include/display_info.h
  ${TOP}/cpp/include/eigenbrot_window.h
  ${TOP}/cpp/include/formula_dialog.h
  ${TOP}/cpp/include/plotter.h
  #${TOP}/cpp/include/scaled_plotter.h
  )
qt4_wrap_ui(${GuiApp}_Uifiles ${TOP}/cpp/ui/formula_dialog.ui)

qt4_add_resources(${GuiApp}_Resources ${TOP}/cpp/source/eigenbrot.qrc)

#add_definitions(-DQT_SHARED -DQT_WEBKIT -D_REENTRANT)
add_definitions(-DQT_SHARED -D_REENTRANT)

set(Language_Resources )
set(Languages de)
foreach(language ${Languages})
  set(language_file cpp/i18n/eigenbroetler_${language}.ts)
  add_custom_command(OUTPUT ${language_file}
                     COMMAND lupdate build/Debug cpp -locations absolute -ts ${language_file}
                     WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

  set(language_resource_file eigenbroetler_${language}.qm)
  add_custom_command(OUTPUT ${language_resource_file}
                     DEPENDS ${language_file}
                     COMMAND lrelease ${language_file} -qm ${language_resource_file}
                     WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
  add_custom_target(${language}_qm_target
                    DEPENDS ${language_resource_file}
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
  list(APPEND Language_Resources ${language}_qm_target)
endforeach(language ${Languages})

add_executable(${GuiApp}
               ${${GuiApp}_Sources}
               ${${GuiApp}_Headers}
               ${${GuiApp}_Mocfiles}
               ${${GuiApp}_Uifiles}
               ${${GuiApp}_Resources})
target_link_libraries(${GuiApp} ${CppLibName})
show(Language_Resources)
add_dependencies(${GuiApp} ${Language_Resources})

set_target_properties(${GuiApp}
  PROPERTIES
  DEBUG_POSTFIX D
  OUTPUT_NAME eigenbroetler
)

if(WithRuby)
  include(ruby_lib)
endif(WithRuby)
