# -*- cmake -*-
# encoding: utf-8

include(FindRuby)

set(RubySource ${TOP}/ruby/ext)

set(RubyLib eigenbroetler_ruby)

set(${RubyLib}_Sources
  ${RubySource}/rb_complex_array.cpp
  )

show(RUBY_INCLUDE_PATH)
show(CPP_INCLUDE_PATH)
include_directories(${RUBY_INCLUDE_PATH} ${CPP_INCLUDE_PATH})

add_library(${RubyLib} SHARED ${${RubyLib}_Sources})
target_link_libraries(${RubyLib} ${RUBY_LIBRARY} ${CppLibName})
set_target_properties(${RubyLib}
  PROPERTIES
  COMPILE_FLAGS "-Wno-shadow"
  PREFIX ""
  LIBRARY_OUTPUT_DIRECTORY ${TOP}/ruby/lib/eigenbrötler
  OUTPUT_NAME eigenbroetler_rb
  )
