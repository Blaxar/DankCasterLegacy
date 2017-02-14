file(GLOB_RECURSE dkc_src_files *.c)

#libdkc
add_library(dkc SHARED ${dkc_src_files})
set_target_properties(dkc PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${DEFAULT_LINK_DIR})

#sources
#add_library(dkc_dummy_source SHARED ${CMAKE_CURRENT_SOURCE_DIR}/sources/dummy.c)
#set_target_properties(dkc_dummy_source PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${DEFAULT_LINK_DIR})
