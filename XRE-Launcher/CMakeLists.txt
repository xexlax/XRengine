project(XRE-Launcher)


file(GLOB_RECURSE HEADER_FILES "*.h")
file(GLOB_RECURSE SOURCE_FILES "*.cpp")

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC
    
    "${ENGINE_SRC_DIR}"
    "${THIRD_PARTY_DIR}"
    "${THIRD_PARTY_INCLUDE}"
)

target_link_libraries(${PROJECT_NAME} XRE-CORE)