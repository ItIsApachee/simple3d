set(SIMPLE3D_EMSCRIPTEN_CMAKE_TOOLCHAIN_FILE "" CACHE FILEPATH "")

# Report first.
simple3d_report_variables(
    "simple3d/simple3d/web"
        SIMPLE3D_EMSCRIPTEN_CMAKE_TOOLCHAIN_FILE 
        SIMPLE3D_WEB_CLIENT_SOURCE_DIR 
        SIMPLE3D_WEB_CLIENT_BINARY_DIR)

# And then check invariants.
if (IS_MAIN_PROJECT AND NOT SIMPLE3D_FORCE_DOWNLOAD_WEB_CLIENT AND (SIMPLE3D_EMSCRIPTEN_CMAKE_TOOLCHAIN_FILE STREQUAL ""))
    # Only required for development.
    message(FATAL_ERROR "Cache variable SIMPLE3D_EMSCRIPTEN_CMAKE_TOOLCHAIN_FILE is empty when library is not used as dependency")
endif()

if (IS_MAIN_PROJECT AND NOT SIMPLE3D_FORCE_DOWNLOAD_WEB_CLIENT)
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
                "-DSIMPLE3D_WEB_CLIENT_GLM_SOURCE_DIR=${SIMPLE3D_WEB_CLIENT_GLM_SOURCE_DIR}"
                "-DSIMPLE3D_WEB_CLIENT_GLM_BINARY_DIR=${SIMPLE3D_WEB_CLIENT_GLM_BINARY_DIR}"
                "-DCMAKE_EXPORT_COMPILE_COMMANDS=ON"
                "-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}"
            INSTALL_COMMAND ""
    )
    simple3d_register_preconfigure_dependency(simple3d_web_client)
else()
    set(SIMPLE3D_WEB_CLIENT_DOWNLOAD_URL ${SIMPLE3D_GITHUB_PREFIX}/${SIMPLE3D_GITHUB_REPO_PATH}/releases/download/${SIMPLE3D_GITHUB_DOWNLOAD_RELEASE_TAG}/${SIMPLE3D_GITHUB_WEB_CLIENT_ASSET_NAME})
    message(STATUS "Simple3D web client download url: ${SIMPLE3D_WEB_CLIENT_DOWNLOAD_URL}")

    ExternalProject_Add(
        simple3d_web_client
            PREFIX "${SIMPLE3D_WEB_CLIENT_BINARY_DIR}"

            SOURCE_DIR "${SIMPLE3D_WEB_CLIENT_BINARY_DIR}/out"  # Since all we need is source code, put it in the out dir right away
            BINARY_DIR "${SIMPLE3D_WEB_CLIENT_BINARY_DIR}"

            TMP_DIR "${SIMPLE3D_WEB_CLIENT_BINARY_DIR}/tmp"
            STAMP_DIR "${SIMPLE3D_WEB_CLIENT_BINARY_DIR}/simple3d-stamp"
            INSTALL_DIR "${SIMPLE3D_WEB_CLIENT_BINARY_DIR}/simple3d-install"
            DOWNLOAD_DIR "${SIMPLE3D_WEB_CLIENT_BINARY_DIR}/simple3d-download" # should be empty

            URL "${SIMPLE3D_WEB_CLIENT_DOWNLOAD_URL}"

            DOWNLOAD_EXTRACT_TIMESTAMP OFF

            CONFIGURE_COMMAND ""
            BUILD_COMMAND ""
            INSTALL_COMMAND ""
    )

endif()
