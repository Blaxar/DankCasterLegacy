file(GLOB DankCaster_src_files ${CMAKE_CURRENT_SOURCE_DIR}/main.c)

add_subdirectory(libdkcst)
IF(CUNIT_FOUND)
	add_subdirectory(unittests)
ELSE(CUNIT_FOUND)
	message("CUnit not found, not generating unit tests.")
ENDIF(CUNIT_FOUND)

#DankCaster executable
add_executable(libDkCst_sample ${DankCaster_src_files})
target_link_libraries(libDkCst_sample pthread dl ${SDL2_LIBRARY} dkcst)
set_target_properties(libDkCst_sample PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${DEFAULT_BIN_DIR})
