cmake_minimum_required (VERSION 3.0)

project ("tsbk07-project")

## Definitions ##

add_definitions(-DGL_GLEXT_PROTOTYPES)
ADD_DEFINITIONS(-DFREEGLUT_STATIC -DGLEW_STATIC)
set(CMAKE_CXX_STANDARD 14)

INCLUDE_DIRECTORIES (external/glew/include external/freeglut/include)

## External libraries ##

#ADD_LIBRARY (freeglut STATIC external/freeglut/lib/freeglut.a)
#ADD_LIBRARY (glew32 STATIC external/glew/lib/glew32.a)
#LINK_DIRECTORIES(external/freeglut/lib external/glew/lib external/freeglut external/glew)
INCLUDE_DIRECTORIES (external/freeglut/include external/glew/include)

# Header only
add_library(eigen INTERFACE)
target_include_directories(eigen INTERFACE external/eigen/include)

## Our libraries ##

add_library(core src/core/GameObject.cpp src/core/Component.cpp src/core/Transform.cpp)
target_include_directories(core PUBLIC include/core)
target_link_libraries(core eigen)

## Executable ##

add_executable(helloworld src/main.cpp)
target_link_libraries(helloworld core 
	${CMAKE_SOURCE_DIR}/external/freeglut/lib/freeglut_static.lib 
	${CMAKE_SOURCE_DIR}/external/glew/lib/glew32s.lib)

## Random hacks ##

# Copy shaders so executable will find it
# I.e put all shaders in src/shaders folder
add_custom_command(TARGET helloworld POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/src/shaders
    $<TARGET_FILE_DIR:helloworld>/shaders)


