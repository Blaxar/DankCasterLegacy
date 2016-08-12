file(GLOB DankCaster_src_files ${CMAKE_CURRENT_SOURCE_DIR}/main.c ${CMAKE_CURRENT_SOURCE_DIR}/libdkcst/*.c)

add_executable(libdkst_test ${DankCaster_src_files})
target_link_libraries(libdkst_test dl)
set_target_properties(libdkst_test PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${DEFAULT_BIN_DIR})

add_subdirectory(libdkcst)
