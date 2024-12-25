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
    ${PROJECT_SOURCE_DIR}/lib/stb
)

# Add subdirectories for external libraries
add_subdirectory(lib/cglm)
add_subdirectory(lib/glad)
add_subdirectory(lib/glfw)

# Find all source files
file(GLOB_RECURSE SRC_FILES src/*.c)

# Add executable
add_executable(${PROJECT_NAME} ${SRC_FILES})

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE cglm glfw glad opengl32 gdi32)