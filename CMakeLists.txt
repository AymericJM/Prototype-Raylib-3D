cmake_minimum_required(VERSION 3.15)
project(PrototypeRaylib VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# to handle the dependencies
# include CPM
include(cmake/CPM.cmake)

# --- DEPENDENCIES ---

# Raylib
CPMAddPackage(
    NAME raylib
    GITHUB_REPOSITORY raysan5/raylib
    GIT_TAG master
    OPTIONS "BUILD_EXAMPLES OFF" # diseable exemples to gain some time
)

# EnTT
CPMAddPackage(
    NAME entt
    GITHUB_REPOSITORY skypjack/entt
    GIT_TAG v3.13.0
)


# config the executable
file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(${PROJECT_NAME} ${SOURCES})


# include headers
target_include_directories(${PROJECT_NAME} PRIVATE include)

# link with libraries
target_link_libraries(${PROJECT_NAME} PRIVATE raylib EnTT::EnTT)

# copy assets to the build folder for the binary
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/assets
    ${CMAKE_CURRENT_BINARY_DIR}/assets)
