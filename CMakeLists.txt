cmake_minimum_required(VERSION 3.14)
project(PacmanClone C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Configure SDL3 options
set(SDL_SHARED OFF CACHE BOOL "" FORCE)
set(SDL_STATIC ON CACHE BOOL "" FORCE)
set(SDL_TEST OFF CACHE BOOL "" FORCE)

# For SDL3_image
set(SDL3_IMAGE_VENDORED ON CACHE BOOL "" FORCE)
set(SDL3_IMAGE_BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

# Add Subdirectories
add_subdirectory(external/SDL3)
add_subdirectory(external/SDL3_image)

file(GLOB SOURCES "src/*.c")
add_executable(${PROJECT_NAME} ${SOURCES})

# Updated Linking
target_link_libraries(${PROJECT_NAME} PRIVATE 
    SDL3::SDL3
    SDL3_image::SDL3_image
)