function(apachee_simple3d_report_variables path)
    message(STATUS "${path} config variables")

    list(APPEND CMAKE_MESSAGE_INDENT "    ")

    foreach(var ${ARGN})
        message(STATUS "${var} = ${${var}}")
    endforeach()

    list(POP_BACK CMAKE_MESSAGE_INDENT)
endfunction()

add_custom_target(apachee_simple3d_preconfigure)
add_custom_target(apachee_simple3d_configure ALL)

add_dependencies(apachee_simple3d_configure apachee_simple3d_preconfigure)

# NB(apachee): Only for targets required for proper configuration for IDE,
# e.g. web client target is required as its compile commands appear only at build step.
function(apachee_simple3d_register_preconfigure_dependency TARGET)
	add_dependencies(apachee_simple3d_preconfigure ${TARGET})
endfunction()

# NB(apachee): Initialize options to ease the development of this project.
function(apachee_simple3d_init_dev_cmake_options)
    set(CMAKE_COMPILE_WARNING_AS_ERROR ON PARENT_SCOPE)

	set(CMAKE_VERBOSE_MAKEFILE ON CACHE BOOL "" FORCE)

	if (MSVC)
		add_compile_options(/W4)

        if (EMSCRIPTEN)
            message(FATAL_ERROR "Expected EMSCRIPTEN compiler, but got ${CMAKE_CXX_COMPILER_ID}")
        endif()
	else()
		add_compile_options(-Wall -Wextra -Wpedantic)
	endif()
endfunction()