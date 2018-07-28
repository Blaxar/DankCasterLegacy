file(GLOB_RECURSE dkc_src_files *.c)

#libdkc
add_library(dkc SHARED ${dkc_src_files})
set_target_properties(dkc PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${DEFAULT_LINK_DIR})
