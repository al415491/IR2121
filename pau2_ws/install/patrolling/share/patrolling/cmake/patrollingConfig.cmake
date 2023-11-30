# generated from ament/cmake/core/templates/nameConfig.cmake.in

# prevent multiple inclusion
if(_patrolling_CONFIG_INCLUDED)
  # ensure to keep the found flag the same
  if(NOT DEFINED patrolling_FOUND)
    # explicitly set it to FALSE, otherwise CMake will set it to TRUE
    set(patrolling_FOUND FALSE)
  elseif(NOT patrolling_FOUND)
    # use separate condition to avoid uninitialized variable warning
    set(patrolling_FOUND FALSE)
  endif()
  return()
endif()
set(_patrolling_CONFIG_INCLUDED TRUE)

# output package information
if(NOT patrolling_FIND_QUIETLY)
  message(STATUS "Found patrolling: 0.0.0 (${patrolling_DIR})")
endif()

# warn when using a deprecated package
if(NOT "" STREQUAL "")
  set(_msg "Package 'patrolling' is deprecated")
  # append custom deprecation text if available
  if(NOT "" STREQUAL "TRUE")
    set(_msg "${_msg} ()")
  endif()
  # optionally quiet the deprecation message
  if(NOT ${patrolling_DEPRECATED_QUIET})
    message(DEPRECATION "${_msg}")
  endif()
endif()

# flag package as ament-based to distinguish it after being find_package()-ed
set(patrolling_FOUND_AMENT_PACKAGE TRUE)

# include all config extra files
set(_extras "")
foreach(_extra ${_extras})
  include("${patrolling_DIR}/${_extra}")
endforeach()
