cmake_minimum_required(VERSION 3.15...3.30)

project(block-task
  DESCRIPTION "AIStatefulTask submodule - Blocking lock task mutex"
  LANGUAGES CXX
)

#==============================================================================
# DEPENDENCIES
#

include(AICxxProject)

#==============================================================================
# BUILD PROJECT
#

# This project is an OBJECT-library, used by other git submodules and the main project.
add_library(block-task_ObjLib OBJECT)

# Require at least support for C++20.
target_compile_features(dbus-task_ObjLib PUBLIC cxx_std_20)

# The list of source files.
target_sources(block-task_ObjLib
  PRIVATE
    "BlockingTaskMutex.cxx"
    "BlockingTaskMutex.h"
)

# Required include search-paths.
get_target_property(CWDS_INTERFACE_INCLUDE_DIRECTORIES AICxx::cwds INTERFACE_INCLUDE_DIRECTORIES)
target_include_directories(block-task_ObjLib
  PUBLIC
    "${CWDS_INTERFACE_INCLUDE_DIRECTORIES}" # For sys.h and debug.h.
)

# Set link dependencies.
target_link_libraries(block-task_ObjLib
  PUBLIC
    AICxx::statefultask
)

# Create an ALIAS target.
add_library(AICxx::block-task ALIAS block-task_ObjLib)
