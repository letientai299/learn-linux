cmake_minimum_required(VERSION 3.17)
project(linux C)

set(CMAKE_C_STANDARD 99)

# dynamically build all folders under "src/cmd" into "bin"
# and keep the relative path of the binaries under "bin" same with "src/cmd".
file(GLOB_RECURSE source_files RELATIVE ${PROJECT_SOURCE_DIR} src/cmd/*/*.c)
foreach (source_file ${source_files})
    # get absolute path of parent folder of the source file into EXE_NAME
    get_filename_component(source_dir ${source_file} DIRECTORY)
    # get only the parent folder name, which is what we want.
    get_filename_component(cmd ${source_dir} NAME)

    # compute the subdir of each command under "bin" directory, so that
    # src/cmd/tools/a/b/c/main.c --> bin/tools/a/b/c
    get_filename_component(cmd_parent ${source_dir} DIRECTORY)
    string(REGEX REPLACE "src/cmd/?" "" bin_subdir ${cmd_parent})

    add_executable(${cmd} ${source_file})
    set_target_properties(
            ${cmd}
            PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/${bin_subdir}"
    )
endforeach (source_file ${source_files})

