IF(CUNIT_FOUND)
	add_subdirectory(unit)
ELSE(CUNIT_FOUND)
	message("CUnit not found, not generating unit tests.")
ENDIF(CUNIT_FOUND)

add_subdirectory(functional)
