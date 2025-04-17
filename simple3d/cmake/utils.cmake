function(report_variables path)
    message(STATUS "${path} config variables")

    list(APPEND CMAKE_MESSAGE_INDENT "    ")

    foreach(var ${ARGN})
        message(STATUS "${var} = ${${var}}")
    endforeach()

    list(POP_BACK CMAKE_MESSAGE_INDENT)
endfunction()

add_custom_target(apachee_simple3d_preconfigure)
add_custom_target(apachee_simple3d_configure)

add_dependencies(apachee_simple3d_configure apachee_simple3d_preconfigure)

function(register_target TARGET)
	add_dependencies(apachee_simple3d_preconfigure ${TARGET})
endfunction()