CMAKE_MINIMUM_REQUIRED (VERSION 3.0)

PROJECT ("tsbk07-project")

## Definitions ##

ADD_DEFINITIONS(-DGL_GLEXT_PROTOTYPES)
ADD_DEFINITIONS(-DFREEGLUT_STATIC -DGLEW_STATIC)
SET(CMAKE_CXX_STANDARD 14)

INCLUDE_DIRECTORIES (external/glew/include external/freeglut/include)

## External libraries ##

#ADD_LIBRARY (freeglut STATIC external/freeglut/lib/freeglut.a)
#ADD_LIBRARY (glew32 STATIC external/glew/lib/glew32.a)
#LINK_DIRECTORIES(external/freeglut/lib external/glew/lib external/freeglut external/glew)
INCLUDE_DIRECTORIES (external/freeglut/include external/glew/include)

# Header only
ADD_LIBRARY(eigen INTERFACE)
TARGET_INCLUDE_DIRECTORIES(eigen INTERFACE external/eigen/include)

## Our libraries ##

ADD_LIBRARY(core src/core/GameObject.cpp src/core/Component.cpp src/core/Transform.cpp)
TARGET_INCLUDE_DIRECTORIES(core PUBLIC include/core)
TARGET_LINK_LIBRARIES(core eigen)

## Executable ##

ADD_EXECUTABLE(helloworld src/main.cpp)
TARGET_LINK_LIBRARIES(helloworld core 
	${CMAKE_SOURCE_DIR}/external/freeglut/lib/freeglut_static.lib 
	${CMAKE_SOURCE_DIR}/external/glew/lib/glew32s.lib)

## Random hacks ##

# Copy shaders so executable will find it
# I.e put all shaders in src/shaders folder
ADD_CUSTOM_COMMAND(TARGET helloworld POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/src/shaders
    $<TARGET_FILE_DIR:helloworld>/shaders)


