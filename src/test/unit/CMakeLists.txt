file(GLOB libdkc_source_test_unit_src_files ${CMAKE_CURRENT_SOURCE_DIR}/dkc_source_test.c)
file(GLOB libdkc_scene_test_unit_src_files ${CMAKE_CURRENT_SOURCE_DIR}/dkc_scene_test.c)
file(GLOB libdkc_param_test_unit_src_files ${CMAKE_CURRENT_SOURCE_DIR}/dkc_param_test.c)
file(GLOB libdkc_sink_test_unit_src_files ${CMAKE_CURRENT_SOURCE_DIR}/dkc_sink_test.c)

#DankCaster unit tests
include_directories(
	${CUNIT_INCLUDE_DIR}
)

add_executable(libdkc_source_test_unit ${libdkc_source_test_unit_src_files})
target_link_libraries(libdkc_source_test_unit dl pthread dkc ${GSTREAMER_LIBRARIES} ${CUNIT_LIBRARIES})
set_target_properties(libdkc_source_test_unit PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${DEFAULT_BIN_DIR})

add_executable(libdkc_scene_test_unit ${libdkc_scene_test_unit_src_files})
target_link_libraries(libdkc_scene_test_unit pthread dl dkc ${GSTREAMER_LIBRARIES} ${CUNIT_LIBRARIES})
set_target_properties(libdkc_scene_test_unit PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${DEFAULT_BIN_DIR})

add_executable(libdkc_param_test_unit ${libdkc_param_test_unit_src_files})
target_link_libraries(libdkc_param_test_unit pthread dl dkc ${GSTREAMER_LIBRARIES} ${CUNIT_LIBRARIES})
set_target_properties(libdkc_param_test_unit PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${DEFAULT_BIN_DIR})

add_executable(libdkc_sink_test_unit ${libdkc_sink_test_unit_src_files})
target_link_libraries(libdkc_sink_test_unit dl pthread dkc ${GSTREAMER_LIBRARIES} ${CUNIT_LIBRARIES})
set_target_properties(libdkc_sink_test_unit PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${DEFAULT_BIN_DIR})
