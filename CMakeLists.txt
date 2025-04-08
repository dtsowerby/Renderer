project(Game)

set(CMAKE_C_STANDARD 11)

# Define compiler flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /std:c11 /O2 /Zi /W4")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /wd4100 /wd4996")  # Adjust these as needed

# Include directories
include_directories(
    ${PROJECT_SOURCE_DIR}/lib/cglm/include/cglm
    ${PROJECT_SOURCE_DIR}/lib/glad/include/glad
    ${PROJECT_SOURCE_DIR}/lib/glfw/include/GLFW
    ${PROJECT_SOURCE_DIR}/lib/glfw/include/cimgui
    ${PROJECT_SOURCE_DIR}/lib/cimgui/imgui
    ${PROJECT_SOURCE_DIR}/lib/cimgui
    ${PROJECT_SOURCE_DIR}/lib/stb
    ${PROJECT_SOURCE_DIR}/src
)

# Add subdirectories for external libraries
add_subdirectory(lib/cglm)
add_subdirectory(lib/glad)
add_subdirectory(lib/glfw)
add_subdirectory(lib/cimgui)

# Find all source files
file(GLOB_RECURSE SRC_FILES src/*.c)

# cimgui library
include_directories(lib/glfw/include)
set(IMGUI_SOURCES 
lib/cimgui/cimgui.cpp
lib/cimgui/imgui/imgui.cpp 
lib/cimgui/imgui/imgui_draw.cpp 
lib/cimgui/imgui/imgui_demo.cpp 
lib/cimgui/imgui/imgui_widgets.cpp
lib/cimgui/imgui/imgui_tables.cpp
lib/cimgui/imgui/backends/imgui_impl_opengl3.cpp
lib/cimgui/imgui/backends/imgui_impl_glfw.cpp
)
add_library(cimgui STATIC ${IMGUI_SOURCES})
add_definitions("-DIMGUI_DISABLE_OBSOLETE_FUNCTIONS=1")
add_compile_definitions("IMGUI_IMPL_OPENGL_LOADER_GL3W")
target_compile_definitions(cimgui PUBLIC "-DIMGUI_IMPL_API=extern \"C\" __declspec\(dllexport\)")
target_link_libraries(cimgui opengl32 glad glfw gdi32)

# Add executable
add_executable(${PROJECT_NAME} ${SRC_FILES})

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE cimgui cglm glfw glad opengl32 gdi32)