# Windows only: URHO3D_WIN32_CONSOLE controls whether to keep console or not
if (NOT URHO3D_WIN32_CONSOLE)
    set (PLATFORM WIN32)
endif ()

set(_src_root_path "${SCPCB_SOURCE_DIR}/Source")
file(
        GLOB_RECURSE SOURCE_FILES
        LIST_DIRECTORIES false
        "${_src_root_path}/*.c*"
        "${_src_root_path}/*.h*"
)

foreach(_source IN ITEMS ${SOURCE_FILES})
    get_filename_component(_source_path "${_source}" PATH)
    file(RELATIVE_PATH _source_path_rel "${_src_root_path}" "${_source_path}")
    string(REPLACE "/" "\\" _group_path "${_source_path_rel}")
    source_group("${_group_path}" FILES "${_source}")
endforeach()

# Add executable.
set (TARGET_NAME ${CMAKE_PROJECT_NAME})
add_executable(${TARGET_NAME} ${PLATFORM} ${SOURCE_FILES})
target_link_libraries (${TARGET_NAME} PRIVATE Urho3D)