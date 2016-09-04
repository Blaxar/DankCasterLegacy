file(GLOB libDkCst_test_src_files ${CMAKE_CURRENT_SOURCE_DIR}/*.c)

#DankCaster unit tests
include_directories(
	${CUNIT_INCLUDE_DIR}
)
add_executable(libDkCst_test ${libDkCst_test_src_files})
target_link_libraries(libDkCst_test dl dkcst ${CUNIT_LIBRARIES})
set_target_properties(libDkCst_test PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${DEFAULT_BIN_DIR})
