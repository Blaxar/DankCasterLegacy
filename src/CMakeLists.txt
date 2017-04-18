file(GLOB DankCaster_src_files ${CMAKE_CURRENT_SOURCE_DIR}/main.c)

add_subdirectory(libdkc)
add_subdirectory(test)

#DankCaster executable
add_executable(libdkc_sample ${DankCaster_src_files})
target_link_libraries(libdkc_sample pthread dl ${SDL2_LIBRARY} ${GSTREAMER_LIBRARIES} dkc)
set_target_properties(libdkc_sample PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${DEFAULT_BIN_DIR})
