function(add_lib_target ARG_LIB_NAME ARG_LIB_VISIBILITY_SPEC)
  add_library(${ARG_LIB_NAME} ${ARG_LIB_VISIBILITY_SPEC})
  
  target_link_libraries(${ARG_LIB_NAME} ${ARG_LIB_VISIBILITY_SPEC}
                ltu::project_options
                ltu::project_warnings)

  #cmake_print_variables(PROJECT_SOURCE_DIR)
  #cmake_print_variables(CMAKE_CURRENT_LIST_DIR)

  set_target_properties(
    ${ARG_LIB_NAME}
    PROPERTIES 
             CXX_VISIBILITY_PRESET hidden
             VISIBILITY_INLINES_HIDDEN YES)


  target_include_directories(${ARG_LIB_NAME} ${WARNING_GUARD} ${ARG_LIB_VISIBILITY_SPEC}
                                $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                                $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
endfunction()


add_lib_target(ltu-header-only INTERFACE)
add_library(ltu::ltu-header-only ALIAS ltu-header-only)

# Request minimum c++ standard, default c++20
target_compile_features(ltu-header-only INTERFACE cxx_std_23)