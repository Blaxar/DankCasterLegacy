file(GLOB libDkCst_source_test_src_files ${CMAKE_CURRENT_SOURCE_DIR}/dkcst_source_test.c)
file(GLOB libDkCst_scene_test_src_files ${CMAKE_CURRENT_SOURCE_DIR}/dkcst_scene_test.c)

#DankCaster unit tests
include_directories(
	${CUNIT_INCLUDE_DIR}
)

add_executable(libDkCst_source_test ${libDkCst_source_test_src_files})
target_link_libraries(libDkCst_source_test dl pthread dkcst ${CUNIT_LIBRARIES})
set_target_properties(libDkCst_source_test PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${DEFAULT_BIN_DIR})

add_executable(libDkCst_scene_test ${libDkCst_scene_test_src_files})
target_link_libraries(libDkCst_scene_test pthread dl dkcst ${CUNIT_LIBRARIES})
set_target_properties(libDkCst_scene_test PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${DEFAULT_BIN_DIR})
