add_subdirectory("internal" "${APACHEE_SIMPLE3D_BINARY_DIR}/simple3d/internal")

add_library(apachee_simple3d STATIC test.cpp)
register_target(apachee_simple3d)

target_include_directories(apachee_simple3d PUBLIC ${APACHEE_SIMPLE3D_SOURCE_DIR})
set_property(TARGET apachee_simple3d PROPERTY CXX_STANDARD 20)

if (IS_MAIN_PROJECT AND CMAKE_EXPORT_COMPILE_COMMANDS)
	block(SCOPE_FOR VARIABLES)
		include(FindPython)
		find_package(Python3 COMPONENTS Interpreter REQUIRED)

		set(APACHEE_SIMPLE3D_COMPILE_COMMANDS_PATH "${APACHEE_SIMPLE3D_BINARY_DIR}/compile_commands.json")

		set(APACHEE_SIMPLE3D_COMPILE_COMMANDS_MARKER_PATH "${APACHEE_SIMPLE3D_BINARY_DIR}/compile_commands.json.marker")

		report_variables(
			"simple3d (compile_commands)"
			APACHEE_SIMPLE3D_COMPILE_COMMANDS_LIST 
			APACHEE_SIMPLE3D_COMPILE_COMMANDS_PATH
			APACHEE_SIMPLE3D_COMPILE_COMMANDS_MARKER_PATH)

		# Initial cleanup
		execute_process(COMMAND ${CMAKE_COMMAND} -E rm -f "${APACHEE_SIMPLE3D_COMPILE_COMMANDS_MARKER_PATH}")
		message(STATUS "Removed ${APACHEE_SIMPLE3D_COMPILE_COMMANDS_MARKER_PATH}")

		add_custom_target(
			apachee_simple3d_merge_compile_commands_target
				${Python_EXECUTABLE} "${APACHEE_SIMPLE3D_SOURCE_DIR}/simple3d/scripts/merge_compile_commands.py"
				"${APACHEE_SIMPLE3D_BINARY_DIR}/compile_commands.json" "${APACHEE_SIMPLE3D_FRONTEND_BINARY_DIR}/compile_commands.json" -O ${APACHEE_SIMPLE3D_COMPILE_COMMANDS_PATH}
				SOURCES "${APACHEE_SIMPLE3D_SOURCE_DIR}/simple3d/scripts/merge_compile_commands.py"
				COMMAND_EXPAND_LISTS
			)

		add_dependencies(apachee_simple3d_merge_compile_commands_target apachee_simple3d_preconfigure)
		add_dependencies(apachee_simple3d_configure apachee_simple3d_merge_compile_commands_target)
	endblock()
endif()
