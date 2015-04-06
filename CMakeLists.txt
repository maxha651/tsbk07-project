cmake_minimum_required (VERSION 3.0)

project ("tsbk07-project")

## Definitions ##

add_definitions(-DGL_GLEXT_PROTOTYPES)
set(CMAKE_CXX_STANDARD 14)

## Dependencies ##

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

## External libraries ##

# Header only
# run configure to get config.hpp (can't get this to work...)
# set(JEAYESON_CONFIG_HPP external/jeayeson/include/jeayeson/config.hpp)
# set_source_files_properties(JEAYESON_CONFIG_HPP PROPERTIES_GENERATED true)
# add_custom_command(OUTPUT JEAYESON_CONFIG_HPP PRE_BUILD COMMAND external/jeayeson/configure)
# add jeayeson lib
add_library(jeayeson INTERFACE)
target_include_directories(jeayeson INTERFACE external/jeayeson/include)

## Our libraries ##

add_library(utils src/utils/VectorUtils3.c src/utils/GL_utilities.c src/utils/loadobj.c
src/utils/JSONLoader.cpp include/utils/JSONLoader.h src/utils/JSONLoader.cpp)
target_include_directories(utils PUBLIC include/utils)
target_link_libraries(utils jeayeson)

add_library(core src/core/GameObject.cpp src/core/Component.cpp src/core/Transform.cpp)
target_include_directories(core PUBLIC include/core)
target_link_libraries(core utils)

## Executable ##

add_executable(helloworld src/main.cpp)
target_link_libraries(helloworld utils
    ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})

## Random hacks ##

# Copy shaders so executable will find it
# I.e put all shaders in src/shaders folder
add_custom_command(TARGET helloworld POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/src/shaders
    $<TARGET_FILE_DIR:helloworld>/shaders)


