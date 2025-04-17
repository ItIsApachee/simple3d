set(APACHEE_SIMPLE3D_EMSCRIPTEN_CMAKE_TOOLCHAIN_FILE "" CACHE FILEPATH "")

# add_subdirectory(library ${APACHEE_SIMPLE3D_SOURCE_DIR})

# TODO(apachee): Add external project for frontend

# Report first.
report_variables(
    "simple3d/simple3d/internal"
        APACHEE_SIMPLE3D_EMSCRIPTEN_CMAKE_TOOLCHAIN_FILE 
        APACHEE_SIMPLE3D_FRONTEND_SOURCE_DIR 
        APACHEE_SIMPLE3D_FRONTEND_BINARY_DIR)

# And then check invariants.
if (IS_MAIN_PROJECT AND (APACHEE_SIMPLE3D_EMSCRIPTEN_CMAKE_TOOLCHAIN_FILE STREQUAL ""))
    # Only required for development.
    message(FATAL_ERROR "Cache variable APACHEE_SIMPLE3D_EMSCRIPTEN_CMAKE_TOOLCHAIN_FILE is empty when library is not used as dependency")
endif()

if (IS_MAIN_PROJECT)
    # TODO(apachee): Prevent creation of 
    ExternalProject_Add(
        apachee_simple3d_frontend
            PREFIX "${APACHEE_SIMPLE3D_FRONTEND_BINARY_DIR}"

            SOURCE_DIR "${APACHEE_SIMPLE3D_FRONTEND_SOURCE_DIR}"
            BINARY_DIR "${APACHEE_SIMPLE3D_FRONTEND_BINARY_DIR}"

            TMP_DIR "${APACHEE_SIMPLE3D_FRONTEND_BINARY_DIR}/tmp"
            STAMP_DIR "${APACHEE_SIMPLE3D_FRONTEND_BINARY_DIR}/simple3d-stamp"
            INSTALL_DIR "${APACHEE_SIMPLE3D_FRONTEND_BINARY_DIR}/simple3d-install"
            DOWNLOAD_DIR "${APACHEE_SIMPLE3D_FRONTEND_BINARY_DIR}/simple3d-download" # should be empty

            BUILD_ALWAYS 1
            CMAKE_ARGS
                -B "${APACHEE_SIMPLE3D_FRONTEND_BINARY_DIR}"
                "-DCMAKE_TOOLCHAIN_FILE=${APACHEE_SIMPLE3D_EMSCRIPTEN_CMAKE_TOOLCHAIN_FILE}"
                "-DAPACHEE_SIMPLE3D_SOURCE_DIR=${APACHEE_SIMPLE3D_SOURCE_DIR}"
                "-DAPACHEE_SIMPLE3D_BINARY_DIR=${APACHEE_SIMPLE3D_BINARY_DIR}"
                "-DCMAKE_EXPORT_COMPILE_COMMANDS=ON"
            INSTALL_COMMAND ""
    )
    register_target(apachee_simple3d_frontend)

endif()

set(APACHEE_SIMPLE3D_COMPILE_COMMANDS_FILE_LIST ${APACHEE_SIMPLE3D_COMPILE_COMMANDS_FILE_LIST} PARENT_SCOPE)
