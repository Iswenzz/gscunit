function(add_test_gscunit)
	set(args NAME PROFILE)
	cmake_parse_arguments(ARG "" "${args}" "" "${ARGN}")

	if(NOT DEFINED ARG_NAME)
		message(FATAL_ERROR "add_test_gscunit() requires the NAME argument.")
	endif()
	if(NOT DEFINED ARG_PROFILE)
		message(FATAL_ERROR "add_test_gscunit() requires the PROFILE argument.")
	endif()

	add_test(
		NAME ${ARG_NAME}
		COMMAND gscunit --profile "${ARG_PROFILE}")
endfunction()
