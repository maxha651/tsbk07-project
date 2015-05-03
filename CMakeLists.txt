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

ENABLE_LANGUAGE (CXX)
SET (CMAKE_CXX_STANDARD 14)
# Disable optimization in debug mode
SET (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")

ADD_DEFINITIONS (-DGL_GLEXT_PROTOTYPES)

# Use these to calculate file paths
ADD_DEFINITIONS (-DTSBK07_PATH=\"${CMAKE_SOURCE_DIR}\")
ADD_DEFINITIONS (-DTSBK07_SRC_PATH=\"${CMAKE_SOURCE_DIR}/src\")
ADD_DEFINITIONS (-DTSBK07_INCLUDE_PATH=\"${CMAKE_SOURCE_DIR}/include\")
ADD_DEFINITIONS (-DTSBK07_SHADERS_PATH=\"${CMAKE_SOURCE_DIR}/src/shaders\")
ADD_DEFINITIONS (-DTSBK07_MODELS_PATH=\"${CMAKE_SOURCE_DIR}/src/models\")
ADD_DEFINITIONS (-DTSBK07_GAMEOBJECTS_PATH=\"${CMAKE_SOURCE_DIR}/gameobjects\")

IF (UNIX)
    # Used by Max' completion engine and syntax checker
    # .h headers are treated as C otherwise
    # Doesn't do anything now since it didn't really do what I wanted
    # Wrong flags being set ?
    SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
ENDIF ()

# Exports compile commands to file
SET (CMAKE_EXPORT_COMPILE_COMMANDS 1)

IF (WIN32)
    # hide insecure CRT warnings, common practice
    ADD_DEFINITIONS (-D_CRT_SECURE_NO_WARNINGS)
    IF (MSVC)
        #SET (CMAKE_SHARED_LINKER_FLAGS ${CMAKE_SHARED_LINKER_FLAGS} "/NODEFAULTLIBS:LIBCMT" )
        SET (CMAKE_DEBUG_POSTFIX "d")
    ENDIF (MSVC)
ENDIF ()

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

# OpenGL should be found even on Windows (amaazing)
FIND_PACKAGE(OpenGL REQUIRED)

INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIRS})
LINK_DIRECTORIES(${OPENGL_LIBRARY_DIRS})
ADD_DEFINITIONS(${OPENGL_DEFINITIONS})
INCLUDE_DIRECTORIES(${GLEW_INCLUDE_DIRS})
LINK_DIRECTORIES(${GLEW_LIBRARY_DIRS})
ADD_DEFINITIONS(${GLEW_DEFINITIONS})
INCLUDE_DIRECTORIES(${GLUT_INCLUDE_DIRS})
LINK_DIRECTORIES(${GLUT_LIBRARY_DIRS})
ADD_DEFINITIONS(${GLUT_DEFINITIONS})

# Boost header only libs
ADD_LIBRARY(boost INTERFACE)
TARGET_INCLUDE_DIRECTORIES(boost INTERFACE external/boost/include)
LINK_LIBRARIES(boost)

# Eigen: Header only
ADD_LIBRARY(eigen INTERFACE)
TARGET_INCLUDE_DIRECTORIES(eigen INTERFACE external/eigen/include)
LINK_LIBRARIES(eigen)

# jsoncpp
ADD_SUBDIRECTORY (external/jsoncpp)
LINK_LIBRARIES (jsoncpp_lib_static)

IF (MSVC)
	# The best way would probably be to use Boost, but this one 
	# is easier. As it turns out, VS is pretty much the only compiler
	# to NOT have it...
	INCLUDE_DIRECTORIES (external/dirent-win)
ENDIF ()

FILE (GLOB_RECURSE SOURCES "${CMAKE_SOURCE_DIR}/src/**.cpp")
FILE (GLOB_RECURSE HEADERS "${CMAKE_SOURCE_DIR}/include/**.h")
FILE (GLOB_RECURSE HEADERS_CPP "${CMAKE_SOURCE_DIR}/include/**.hpp")
FILE (GLOB_RECURSE SHADERS "${CMAKE_SOURCE_DIR}/src/shaders/**")
FILE (GLOB_RECURSE MODELS "${CMAKE_SOURCE_DIR}/src/models/**")

# Make it look nicer in VS
# Will add put all shaders in "shaders" folder etc.
SOURCE_GROUP (shaders FILES ${SHADERS})
SOURCE_GROUP (models FILES ${MODELS})

INCLUDE_DIRECTORIES(PUBLIC include/core)
INCLUDE_DIRECTORIES(PUBLIC include)
 
## Executable ##

ADD_EXECUTABLE(helloworld ${SOURCES} ${HEADERS} ${HEADERS_CPP} ${SHADERS})
TARGET_LINK_LIBRARIES(helloworld
    ${GLUT_LIBRARY} ${GLEW_LIBRARY} ${OPENGL_LIBRARIES})

