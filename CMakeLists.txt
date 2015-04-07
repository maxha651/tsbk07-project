cmake_minimum_required (VERSION 3.0)

project ("tsbk07-project")

## Definitions ##

add_definitions(-DGL_GLEXT_PROTOTYPES)
set(CMAKE_CXX_STANDARD 14)

IF (!Windows)
## Dependencies ##
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})
link_directories(${GLEW_LIBRARY_DIRS})
add_definitions(${GLEW_DEFINITIONS})

# OpenGL
find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})

# FreeGLUT
find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIRS})
link_directories(${GLUT_LIBRARY_DIRS})
add_definitions(${GLUT_DEFINITIONS})
ENDIF ()

## External libraries ##

# Header only
add_library(eigen INTERFACE)
target_include_directories(eigen INTERFACE external/eigen/include)

## Our libraries ##

add_library(core src/core/GameObject.cpp src/core/Component.cpp src/core/Transform.cpp)
target_include_directories(core PUBLIC include/core)
target_link_libraries(core eigen)

## Executable ##

add_executable(helloworld src/main.cpp)
target_link_libraries(helloworld core)

## Random hacks ##

# Copy shaders so executable will find it
# I.e put all shaders in src/shaders folder
add_custom_command(TARGET helloworld POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/src/shaders
    $<TARGET_FILE_DIR:helloworld>/shaders)


