add_library(dkcst_dummy_source SHARED ${CMAKE_CURRENT_SOURCE_DIR}/sources/dummy.c)
set_target_properties(dkcst_dummy_source PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${DEFAULT_LINK_DIR})
