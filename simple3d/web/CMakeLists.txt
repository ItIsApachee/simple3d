set(SIMPLE3D_EMSCRIPTEN_CMAKE_TOOLCHAIN_FILE "" CACHE FILEPATH "")

# TODO(apachee): Add external project for web_client

# Report first.
simple3d_report_variables(
    "simple3d/simple3d/web"
        SIMPLE3D_EMSCRIPTEN_CMAKE_TOOLCHAIN_FILE 
        SIMPLE3D_WEB_CLIENT_SOURCE_DIR 
        SIMPLE3D_WEB_CLIENT_BINARY_DIR)

# And then check invariants.
if (IS_MAIN_PROJECT AND (SIMPLE3D_EMSCRIPTEN_CMAKE_TOOLCHAIN_FILE STREQUAL ""))
    # Only required for development.
    message(FATAL_ERROR "Cache variable SIMPLE3D_EMSCRIPTEN_CMAKE_TOOLCHAIN_FILE is empty when library is not used as dependency")
endif()

if (IS_MAIN_PROJECT)
    # TODO(apachee): Prevent creation of 

    ExternalProject_Add(
        simple3d_web_client
            PREFIX "${SIMPLE3D_WEB_CLIENT_BINARY_DIR}"

            SOURCE_DIR "${SIMPLE3D_WEB_CLIENT_SOURCE_DIR}"
            BINARY_DIR "${SIMPLE3D_WEB_CLIENT_BINARY_DIR}"

            TMP_DIR "${SIMPLE3D_WEB_CLIENT_BINARY_DIR}/tmp"
            STAMP_DIR "${SIMPLE3D_WEB_CLIENT_BINARY_DIR}/simple3d-stamp"
            INSTALL_DIR "${SIMPLE3D_WEB_CLIENT_BINARY_DIR}/simple3d-install"
            DOWNLOAD_DIR "${SIMPLE3D_WEB_CLIENT_BINARY_DIR}/simple3d-download" # should be empty

            BUILD_ALWAYS 1
            CMAKE_ARGS
                -B "${SIMPLE3D_WEB_CLIENT_BINARY_DIR}"
                "-DCMAKE_TOOLCHAIN_FILE=${SIMPLE3D_EMSCRIPTEN_CMAKE_TOOLCHAIN_FILE}"
                "-DSIMPLE3D_SOURCE_DIR=${SIMPLE3D_SOURCE_DIR}"
                "-DSIMPLE3D_BINARY_DIR=${SIMPLE3D_BINARY_DIR}"
                "-DCMAKE_EXPORT_COMPILE_COMMANDS=ON"
                "-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}"
            INSTALL_COMMAND ""
    )
    simple3d_register_preconfigure_dependency(simple3d_web_client)

else()
    message(FATAL_ERROR "Loading web_client part of this library is WIP")
endif()

set(SIMPLE3D_COMPILE_COMMANDS_FILE_LIST ${SIMPLE3D_COMPILE_COMMANDS_FILE_LIST} PARENT_SCOPE)
