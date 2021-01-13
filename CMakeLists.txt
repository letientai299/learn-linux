cmake_minimum_required(VERSION 3.17)

project(linux)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_COLOR_MAKEFILE ON)

# ------------------------------------------------------------------------------
# config ccache for faster rebuild
option(AUTO_CCACHE "Use ccache to speed up rebuilds" ON)
find_program(CCACHE_PROGRAM ccache)
if (CCACHE_PROGRAM AND ${AUTO_CCACHE})
  message(STATUS "Using ${CCACHE_PROGRAM} as compiler launcher")
  set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
  # requires at least CMake 3.9 to be any use
  set(CMAKE_CUDA_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
endif ()

# ------------------------------------------------------------------------------
# config for external dependencies

# config CPM package manager
include(cmake/CPM.cmake)
set(CPM_SOURCE_CACHE ${PROJECT_SOURCE_DIR}/_deps)

CPMAddPackage(
        NAME doctest
        GITHUB_REPOSITORY onqtam/doctest
        GIT_TAG 2.4.4
)
include(${doctest_SOURCE_DIR}/scripts/cmake/doctest.cmake)

# ------------------------------------------------------------------------------
# add all code under "src/lib" into to libraries.
file(
        GLOB_RECURSE lib_sources RELATIVE ${PROJECT_SOURCE_DIR}
        src/lib/*/*.h
        src/lib/*/*.c
)
include_directories(${PROJECT_SOURCE_DIR}/src)

# ------------------------------------------------------------------------------
# dynamically build all folders under "src/cmd" into "bin"
# and keep the relative path of the binaries under "bin" same with "src/cmd".
file(GLOB_RECURSE app_sources RELATIVE ${PROJECT_SOURCE_DIR} src/cmd/*/*.c)
foreach (source_file ${app_sources})
  # get absolute path of parent folder of the source file into EXE_NAME
  get_filename_component(source_dir ${source_file} DIRECTORY)
  # get only the parent folder name, which is what we want.
  get_filename_component(cmd ${source_dir} NAME)

  # compute the subdir of each command under "bin" directory, so that
  # src/cmd/tools/a/b/c/main.c --> bin/tools/a/b/c
  get_filename_component(cmd_parent ${source_dir} DIRECTORY)
  string(REGEX REPLACE "src/cmd/?" "" bin_subdir ${cmd_parent})

  add_executable(${cmd} ${source_file} ${lib_sources})
  target_link_libraries(${cmd} ${CPM_PACKAGES})
  set_target_properties(
          ${cmd}
          PROPERTIES
          RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/${bin_subdir}"
  )
endforeach (source_file ${app_sources})


# ------------------------------------------------------------------------------
# config to run all tests in "test" folder

# file all test sources
file(GLOB_RECURSE test_sources RELATIVE ${PROJECT_SOURCE_DIR} test/*.*)
file(
        GLOB_RECURSE lib_copied_sources RELATIVE ${PROJECT_SOURCE_DIR}
        bin/copied/*/*.c
)
set_source_files_properties(${lib_copied_sources} PROPERTIES LANGUAGE CXX)

add_custom_target(
        symlink
        COMMAND mkdir -p ${PROJECT_SOURCE_DIR}/bin/copied
        COMMAND rsync -au --delete ${PROJECT_SOURCE_DIR}/src/lib ${PROJECT_SOURCE_DIR}/bin/copied/
)

add_executable(tester ${test_sources} ${lib_copied_sources})
add_dependencies(tester symlink)

target_link_libraries(tester ${CPM_PACKAGES})
doctest_discover_tests(
        tester
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/test/"
)

# enable ctest
enable_testing()

set_target_properties(
        tester
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/test/"
)
add_custom_target(
        check
        COMMAND CTEST_OUTPUT_ON_FAILURE=1 ${CMAKE_CTEST_COMMAND}
        DEPENDS tester
)

# ------------------------------------------------------------------------------
# custom make clean-all to remove all cmake generated files
add_custom_target(
        clean-all
        COMMAND ${CMAKE_BUILD_TOOL} clean
        COMMAND rm cmake_install.cmake
        COMMAND rm CMakeCache.txt
        COMMAND rm -rf CPM_modules
        COMMAND rm -rf Testing
        COMMAND rm -rf bin
        COMMAND rm -rf CMakeFiles
        COMMAND rm -rf cmake-build-*
        COMMAND rm -rf *.cbp
        COMMAND rm -rf *.cmake
        COMMAND rm -rf Makefile
)


#get_cmake_property(_variableNames VARIABLES)
#list(SORT _variableNames)
#foreach (_variableName ${_variableNames})
#  message(STATUS "${_variableName}=${${_variableName}}")
#endforeach ()
