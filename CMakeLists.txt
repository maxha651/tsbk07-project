CMAKE_MINIMUM_REQUIRED (VERSION 3.0)

## Some explanations
# 
# PROJECT() is only for metadata about project. We can have a lot of more
# stuff like version number etc. but not needed for us.
# 
# ADD_DEFINITIONS() is for setting defines that are accessible from code.
# -DGL_GLEXT_PROTOTYPES makes #ifdef GL_GLEXT_PROTOTYPES compile. If you're
# confused about -D prefix, then see gcc man pages or something.
# 
# SET() is used for setting variables in CMake. These will be used in this 
# file only and are NOT accessible from code.
# 
# FIND_PACKAGE() searches for some package and sets appropriate variables.
# The usage of this should be fairly obvious from code below.
# 
# INCLUDE_DIRECTORIES() adds a directory to search path for header files.
# if we add .../include/ to this then .../include/header.h will be found 
# when using #include <header.h>.
# 
# LINK_DIRECTORIES() is (I think) used for finding .dll files when using
# dynamic linking.
# 
# ADD_LIBRARY() and ADD_EXECUTABLE() adds another compile target. Arguments
# is sources. Library will generate a library and executable will generate
# an executable.
# 
# TARGET_LINK_DIRECTORIES() is used for the linker. After a target has compiled,
# the linker will link in the specified libraries. These can be precompiled 
# libraries or libraries that has been compiled by ADD_LIBRARY() in a previous 
# step. Linux uses suffix .so for dynamic libraries and .a for static libraries.
# Windows uses .lib for both so how the linker knows what's what there I really
# have no idea.
#


PROJECT ("tsbk07-project")

## Definitions and settings ##

ADD_DEFINITIONS(-DGL_GLEXT_PROTOTYPES)
SET(CMAKE_CXX_STANDARD 14)

## External libraries ##

# Keeping Windows dependencies in repository. Too much hassle otherwise.
# The variables being set here is set automagically by find_package.
# If adding other compiled libs in repository, this is the way to do it.
# There shouldn't be any difference between our hack and find_package.
IF (WIN32)
    SET (GLUT_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/external/freeglut/include)
    SET (GLEW_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/external/glew/include)
    SET (GLUT_LIBRARY_DIRS ${CMAKE_SOURCE_DIR}/external/freeglut)
    SET (GLEW_LIBRARY_DIRS ${CMAKE_SOURCE_DIR}/external/glew)
    SET (GLUT_LIBRARY ${CMAKE_SOURCE_DIR}/external/freeglut/lib/freeglut_static.lib)
    SET (GLEW_LIBRARY ${CMAKE_SOURCE_DIR}/external/glew/lib/glew32s.lib)
    SET (GLUT_DEFINITIONS -DFREEGLUT_STATIC)
    SET (GLEW_DEFINITIONS -DGLEW_STATIC)
ELSE ()
    FIND_PACKAGE(GLEW REQUIRED)
    FIND_PACKAGE(GLUT REQUIRED)
ENDIF ()

# OpenGL should found even on Windows (amaazing)
FIND_PACKAGE(OpenGL REQUIRED)

include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})
include_directories(${GLEW_INCLUDE_DIRS})
link_directories(${GLEW_LIBRARY_DIRS})
add_definitions(${GLEW_DEFINITIONS})
include_directories(${GLUT_INCLUDE_DIRS})
link_directories(${GLUT_LIBRARY_DIRS})
add_definitions(${GLUT_DEFINITIONS})


# Eigen: Header only
ADD_LIBRARY(eigen INTERFACE)
TARGET_INCLUDE_DIRECTORIES(eigen INTERFACE external/eigen/include)

## Our libraries ##

# XXX:  Might just keep a list of source files here as well if we want.
#       Then we just add all sources to a SOURCES variable and add use in add_executable.
#       Separate libraries looks nicer though and might improve compile time.

ADD_LIBRARY(core src/core/GameObject.cpp src/core/Component.cpp src/core/Transform.cpp)
TARGET_INCLUDE_DIRECTORIES(core PUBLIC include/core)
TARGET_LINK_LIBRARIES(core eigen)

## Executable ##

ADD_EXECUTABLE(helloworld src/main.cpp)
TARGET_LINK_LIBRARIES(helloworld core 
    ${GLUT_LIBRARY} ${GLEW_LIBRARY} ${OPENGL_LIBRARIES})

## Random hacks ##

# Copy shaders so executable will find it
# I.e put all shaders in src/shaders folder
ADD_CUSTOM_COMMAND(TARGET helloworld POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/src/shaders
    $<TARGET_FILE_DIR:helloworld>/shaders)


