file(GLOB dkcst_src_files ${CMAKE_CURRENT_SOURCE_DIR}/*.c)

#libdkcst
add_library(dkcst SHARED ${dkcst_src_files})
set_target_properties(dkcst PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${DEFAULT_LINK_DIR})

#sources
add_library(DkCst_dummy_source SHARED ${CMAKE_CURRENT_SOURCE_DIR}/sources/dummy.c)
set_target_properties(DkCst_dummy_source PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${DEFAULT_LINK_DIR})
