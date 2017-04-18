
add_executable(libdkc_dummy_test_func ${CMAKE_CURRENT_SOURCE_DIR}/dkc_dummy_test.c)
target_link_libraries(libdkc_dummy_test_func dl pthread dkc ${GSTREAMER_LIBRARIES})
set_target_properties(libdkc_dummy_test_func PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${DEFAULT_BIN_DIR})
