cmake_minimum_required(VERSION 3.23)

# Only set the cxx_standard if it is not set by someone else
if(NOT DEFINED CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 20)    
endif()

set(CXX_STANDARD_REQUIRED True)

# strongly encouraged to enable this globally to avoid conflicts between
# -Wpedantic being enabled and -std=c++20 and -std=gnu++20 for example
# when compiling with PCH enabled
set(CMAKE_CXX_EXTENSIONS OFF)

project(
    LifetimeLog
    DESCRIPTION "Simple lifetime logging proxy object"
    LANGUAGES CXX
)

include(cmake/PreventInSourceBuilds.cmake)
include(ProjectOptions.cmake)

myproject_setup_options()

myproject_global_options()

include(Dependencies.cmake)
myproject_setup_dependencies()

myproject_local_options()

target_compile_features(myproject_options INTERFACE cxx_std_${CMAKE_CXX_STANDARD})

add_library(ltu::project_options ALIAS myproject_options)
add_library(ltu::project_warnings ALIAS myproject_warnings)

#--------------------------------------------------------------------
# Add subdirectories
#--------------------------------------------------------------------
add_subdirectory(src)

if (myproject_BUILD_EXAMPLE)
    add_subdirectory(examples)
endif()


# Don't even look at tests if we're not top level
if(NOT PROJECT_IS_TOP_LEVEL)
  return()
endif()

if (BUILD_TESTING)
    add_subdirectory(tests)
endif()

# If MSVC is being used, and ASAN is enabled, we need to set the debugger environment
# so that it behaves well with MSVC's debugger, and we can run the target from visual studio
if(MSVC)
  get_all_installable_targets(all_targets)
  message("all_targets=${all_targets}")
  set_target_properties(${all_targets} PROPERTIES VS_DEBUGGER_ENVIRONMENT "PATH=$(VC_ExecutablePath_x64);%PATH%")
endif()

# set the startup project for the "play" button in MSVC
if (myproject_BUILD_EXAMPLE AND MSVC)
  set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT demo_app)
endif()
