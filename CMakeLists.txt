cmake_minimum_required(VERSION 3.14)
project(PacmanClone C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ------------------------------
# Address sanitizer
# ------------------------------
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address -g")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")

# ------------------------------
# SDL3 static configuration
# ------------------------------
set(SDL3_SHARED ON CACHE BOOL "" FORCE)
set(SDL3_STATIC OFF CACHE BOOL "" FORCE)

set(SDL3IMAGE_SHARED ON CACHE BOOL "" FORCE)
set(SDL3IMAGE_STATIC OFF CACHE BOOL "" FORCE)


# SDL3_image static configuration
set(SDL3_IMAGE_VENDORED ON CACHE BOOL "" FORCE)
set(SDL3_IMAGE_BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

# ------------------------------
# Add SDL3 and SDL3_image subdirectories
# ------------------------------
add_subdirectory(external/SDL3)
add_subdirectory(external/SDL3_image)

# ------------------------------
# Collect source files
# ------------------------------
file(GLOB SOURCES "src/*.c")

# ------------------------------
# Create executable
# ------------------------------
add_executable(${PROJECT_NAME} ${SOURCES})

# Force output to project root
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}
)

# ------------------------------
# Link SDL libraries
# ------------------------------
target_link_libraries(${PROJECT_NAME} PRIVATE
    SDL3::SDL3
    SDL3_image::SDL3_image
)

# ------------------------------
# Copy assets folder next to executable
# ------------------------------
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets ${CMAKE_SOURCE_DIR}/assets
)
