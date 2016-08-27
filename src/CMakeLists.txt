file(GLOB DankCaster_src_files ${CMAKE_CURRENT_SOURCE_DIR}/main.c ${CMAKE_CURRENT_SOURCE_DIR}/libDkCst/*.c)

add_executable(libDkCst_test ${DankCaster_src_files})
target_link_libraries(libDkCst_test dl ${SDL2_LIBRARY})
set_target_properties(libDkCst_test PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${DEFAULT_BIN_DIR})

add_subdirectory(libDkCst)
