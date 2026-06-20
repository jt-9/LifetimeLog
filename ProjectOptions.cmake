include(cmake/SystemLink.cmake)
include(cmake/LibFuzzer.cmake)
include(CMakeDependentOption)
include(CheckCXXCompilerFlag)


include(CheckCXXSourceCompiles)


macro(ltu_supports_sanitizers)
  if((CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*" OR CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*") AND NOT WIN32)

    message(STATUS "Sanity checking UndefinedBehaviorSanitizer, it should be supported on this platform")
    set(TEST_PROGRAM "int main() { return 0; }")

    # Check if UndefinedBehaviorSanitizer works at link time
    set(CMAKE_REQUIRED_FLAGS "-fsanitize=undefined")
    set(CMAKE_REQUIRED_LINK_OPTIONS "-fsanitize=undefined")
    check_cxx_source_compiles("${TEST_PROGRAM}" HAS_UBSAN_LINK_SUPPORT)
    
    if(HAS_UBSAN_LINK_SUPPORT)
      message(STATUS "UndefinedBehaviorSanitizer is supported at both compile and link time.")
      set(SUPPORTS_UBSAN ON)
    else()
      message(WARNING "UndefinedBehaviorSanitizer is NOT supported at link time.")
      set(SUPPORTS_UBSAN OFF)
    endif()
  else()
    set(SUPPORTS_UBSAN OFF)
  endif()

  if((CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*" OR CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*") AND WIN32)
    set(SUPPORTS_ASAN OFF)
  else()
    if (NOT WIN32)
      message(STATUS "Sanity checking AddressSanitizer, it should be supported on this platform")
      set(TEST_PROGRAM "int main() { return 0; }")

      # Check if AddressSanitizer works at link time
      set(CMAKE_REQUIRED_FLAGS "-fsanitize=address")
      set(CMAKE_REQUIRED_LINK_OPTIONS "-fsanitize=address")
      check_cxx_source_compiles("${TEST_PROGRAM}" HAS_ASAN_LINK_SUPPORT)

      if(HAS_ASAN_LINK_SUPPORT)
        message(STATUS "AddressSanitizer is supported at both compile and link time.")
        set(SUPPORTS_ASAN ON)
      else()
        message(WARNING "AddressSanitizer is NOT supported at link time.")
        set(SUPPORTS_ASAN OFF)
      endif()
    else()
      set(SUPPORTS_ASAN ON)
    endif()
  endif()
endmacro()

macro(ltu_setup_options)
  option(ltu_ENABLE_HARDENING "Enable hardening" ON)
  option(ltu_ENABLE_COVERAGE "Enable coverage reporting" OFF)
  cmake_dependent_option(
    ltu_ENABLE_GLOBAL_HARDENING
    "Attempt to push hardening options to built dependencies"
    ON
    ltu_ENABLE_HARDENING
    OFF)

  ltu_supports_sanitizers()

  if(NOT PROJECT_IS_TOP_LEVEL OR ltu_PACKAGING_MAINTAINER_MODE)
    option(ltu_ENABLE_IPO "Enable IPO/LTO" OFF)
    option(ltu_WARNINGS_AS_ERRORS "Treat Warnings As Errors" OFF)
    option(ltu_ENABLE_USER_LINKER "Enable user-selected linker" OFF)
    option(ltu_ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" OFF)
    option(ltu_ENABLE_SANITIZER_LEAK "Enable leak sanitizer" OFF)
    option(ltu_ENABLE_SANITIZER_UNDEFINED "Enable undefined sanitizer" OFF)
    option(ltu_ENABLE_SANITIZER_THREAD "Enable thread sanitizer" OFF)
    option(ltu_ENABLE_SANITIZER_MEMORY "Enable memory sanitizer" OFF)
    option(ltu_ENABLE_UNITY_BUILD "Enable unity builds" OFF)
    option(ltu_ENABLE_CLANG_TIDY "Enable clang-tidy" OFF)
    option(ltu_ENABLE_CPPCHECK "Enable cpp-check analysis" OFF)
    option(ltu_ENABLE_PCH "Enable precompiled headers" OFF)
    option(ltu_ENABLE_CACHE "Enable ccache" OFF)
    option(ltu_BUILD_EXAMPLE "Build example application" OFF)
  else()
    option(ltu_ENABLE_IPO "Enable IPO/LTO" ON)
    option(ltu_WARNINGS_AS_ERRORS "Treat Warnings As Errors" ON)
    option(ltu_ENABLE_USER_LINKER "Enable user-selected linker" OFF)
    option(ltu_ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" ${SUPPORTS_ASAN})
    option(ltu_ENABLE_SANITIZER_LEAK "Enable leak sanitizer" OFF)
    option(ltu_ENABLE_SANITIZER_UNDEFINED "Enable undefined sanitizer" ${SUPPORTS_UBSAN})
    option(ltu_ENABLE_SANITIZER_THREAD "Enable thread sanitizer" OFF)
    option(ltu_ENABLE_SANITIZER_MEMORY "Enable memory sanitizer" OFF)
    option(ltu_ENABLE_UNITY_BUILD "Enable unity builds" OFF)
    option(ltu_ENABLE_CLANG_TIDY "Enable clang-tidy" ON)
    option(ltu_ENABLE_CPPCHECK "Enable cpp-check analysis" ON)
    option(ltu_ENABLE_PCH "Enable precompiled headers" OFF)
    option(ltu_ENABLE_CACHE "Enable ccache" ON)
    option(ltu_BUILD_EXAMPLE "Build example application" ON)
  endif()

  if(NOT PROJECT_IS_TOP_LEVEL)
    mark_as_advanced(
      ltu_ENABLE_IPO
      ltu_WARNINGS_AS_ERRORS
      ltu_ENABLE_USER_LINKER
      ltu_ENABLE_SANITIZER_ADDRESS
      ltu_ENABLE_SANITIZER_LEAK
      ltu_ENABLE_SANITIZER_UNDEFINED
      ltu_ENABLE_SANITIZER_THREAD
      ltu_ENABLE_SANITIZER_MEMORY
      ltu_ENABLE_UNITY_BUILD
      ltu_ENABLE_CLANG_TIDY
      ltu_ENABLE_CPPCHECK
      ltu_ENABLE_COVERAGE
      ltu_ENABLE_PCH
      ltu_ENABLE_CACHE
      )
  endif()

  ltu_check_libfuzzer_support(LIBFUZZER_SUPPORTED)
  if(LIBFUZZER_SUPPORTED AND (ltu_ENABLE_SANITIZER_ADDRESS OR ltu_ENABLE_SANITIZER_THREAD OR ltu_ENABLE_SANITIZER_UNDEFINED))
    set(DEFAULT_FUZZER ON)
  else()
    set(DEFAULT_FUZZER OFF)
  endif()

  option(ltu_BUILD_FUZZ_TESTS "Enable fuzz testing executable" ${DEFAULT_FUZZER})

endmacro()

macro(ltu_global_options)
  if(ltu_ENABLE_IPO)
    include(cmake/InterproceduralOptimization.cmake)
    ltu_enable_ipo()
  endif()

  ltu_supports_sanitizers()

  if(ltu_ENABLE_HARDENING AND ltu_ENABLE_GLOBAL_HARDENING)
    include(cmake/Hardening.cmake)
    if(NOT SUPPORTS_UBSAN 
       OR ltu_ENABLE_SANITIZER_UNDEFINED
       OR ltu_ENABLE_SANITIZER_ADDRESS
       OR ltu_ENABLE_SANITIZER_THREAD
       OR ltu_ENABLE_SANITIZER_LEAK)
      set(ENABLE_UBSAN_MINIMAL_RUNTIME FALSE)
    else()
      set(ENABLE_UBSAN_MINIMAL_RUNTIME TRUE)
    endif()
    message("${ltu_ENABLE_HARDENING} ${ENABLE_UBSAN_MINIMAL_RUNTIME} ${ltu_ENABLE_SANITIZER_UNDEFINED}")
    ltu_enable_hardening(ltu_project_options ON ${ENABLE_UBSAN_MINIMAL_RUNTIME})
  endif()
endmacro()

macro(ltu_local_options)
  if(PROJECT_IS_TOP_LEVEL)
    include(cmake/StandardProjectSettings.cmake)
  endif()

  add_library(ltu_project_warnings INTERFACE)
  add_library(ltu_project_options INTERFACE)

  include(cmake/CompilerWarnings.cmake)
  ltu_set_project_warnings(
    ltu_project_warnings
    ${ltu_WARNINGS_AS_ERRORS}
    ""
    ""
    ""
    "")

  if(ltu_ENABLE_USER_LINKER)
    include(cmake/Linker.cmake)
    ltu_configure_linker(ltu_project_options)
  endif()

  include(cmake/Sanitizers.cmake)
  ltu_enable_sanitizers(
    ltu_project_options
    ${ltu_ENABLE_SANITIZER_ADDRESS}
    ${ltu_ENABLE_SANITIZER_LEAK}
    ${ltu_ENABLE_SANITIZER_UNDEFINED}
    ${ltu_ENABLE_SANITIZER_THREAD}
    ${ltu_ENABLE_SANITIZER_MEMORY})

  set_target_properties(ltu_project_options PROPERTIES UNITY_BUILD ${ltu_ENABLE_UNITY_BUILD})

  if(ltu_ENABLE_PCH)
    target_precompile_headers(
      ltu_project_options
      INTERFACE
      <vector>
      <string>
      <utility>)
  endif()

  if(ltu_ENABLE_CACHE)
    include(cmake/Cache.cmake)
    ltu_enable_cache()
  endif()

  include(cmake/StaticAnalyzers.cmake)
  if(ltu_ENABLE_CLANG_TIDY)
    ltu_enable_clang_tidy(ltu_project_options ${ltu_WARNINGS_AS_ERRORS})
  endif()

  if(ltu_ENABLE_CPPCHECK)
    ltu_enable_cppcheck(${ltu_WARNINGS_AS_ERRORS} "" # override cppcheck options
    )
  endif()

  if(ltu_ENABLE_COVERAGE)
    include(cmake/Tests.cmake)
    ltu_enable_coverage(ltu_project_options)
  endif()

  if(ltu_WARNINGS_AS_ERRORS)
    check_cxx_compiler_flag("-Wl,--fatal-warnings" LINKER_FATAL_WARNINGS)
    if(LINKER_FATAL_WARNINGS)
      # This is not working consistently, so disabling for now
      # target_link_options(ltu_project_options INTERFACE -Wl,--fatal-warnings)
    endif()
  endif()

  if(ltu_ENABLE_HARDENING AND NOT ltu_ENABLE_GLOBAL_HARDENING)
    include(cmake/Hardening.cmake)
    if(NOT SUPPORTS_UBSAN 
       OR ltu_ENABLE_SANITIZER_UNDEFINED
       OR ltu_ENABLE_SANITIZER_ADDRESS
       OR ltu_ENABLE_SANITIZER_THREAD
       OR ltu_ENABLE_SANITIZER_LEAK)
      set(ENABLE_UBSAN_MINIMAL_RUNTIME FALSE)
    else()
      set(ENABLE_UBSAN_MINIMAL_RUNTIME TRUE)
    endif()
    ltu_enable_hardening(ltu_project_options OFF ${ENABLE_UBSAN_MINIMAL_RUNTIME})
  endif()

endmacro()
