add_subdirectory(core)
add_subdirectory(web)

if (IS_MAIN_PROJECT AND CMAKE_EXPORT_COMPILE_COMMANDS AND NOT SIMPLE3D_FORCE_DOWNLOAD_WEB_CLIENT)
	block(SCOPE_FOR VARIABLES)
		include(FindPython)
		find_package(Python3 COMPONENTS Interpreter REQUIRED)

		set(SIMPLE3D_COMPILE_COMMANDS_PATH "${SIMPLE3D_BINARY_DIR}/compile_commands.json")

		set(SIMPLE3D_COMPILE_COMMANDS_MARKER_PATH "${SIMPLE3D_BINARY_DIR}/compile_commands.json.marker")

		simple3d_report_variables(
			"simple3d (compile_commands)"
			SIMPLE3D_COMPILE_COMMANDS_PATH
			SIMPLE3D_COMPILE_COMMANDS_MARKER_PATH)

		# Initial cleanup
		execute_process(COMMAND ${CMAKE_COMMAND} -E rm -f "${SIMPLE3D_COMPILE_COMMANDS_MARKER_PATH}")
		message(STATUS "Removed ${SIMPLE3D_COMPILE_COMMANDS_MARKER_PATH}")

		add_custom_target(
			simple3d_merge_compile_commands_target
				${Python_EXECUTABLE} "${SIMPLE3D_SOURCE_DIR}/simple3d/scripts/merge_compile_commands.py"
				"${SIMPLE3D_BINARY_DIR}/compile_commands.json" "${SIMPLE3D_WEB_CLIENT_BINARY_DIR}/compile_commands.json" -O ${SIMPLE3D_COMPILE_COMMANDS_PATH}
				SOURCES "${SIMPLE3D_SOURCE_DIR}/simple3d/scripts/merge_compile_commands.py"
				COMMAND_EXPAND_LISTS
			)

		add_dependencies(simple3d_merge_compile_commands_target simple3d_preconfigure)
		add_dependencies(simple3d_configure simple3d_merge_compile_commands_target)
	endblock()
endif()
