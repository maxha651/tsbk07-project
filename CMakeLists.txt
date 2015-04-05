cmake_minimum_required (VERSION 2.8)

add_definitions(-DGL_GLEXT_PROTOTYPES)

project ("tsbk07-project")

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

include_directories(include)

# Set up libraries which can be linked with executable
# Another way to do this might be to just link all sources to executable
# Not sure what is best here
add_library(VectorUtils3 src/utils/VectorUtils3.c)
add_library(GL_utilities src/utils/GL_utilities.c)
add_library(loadobj src/utils/loadobj.c)

add_executable(helloworld src/main.cpp)

target_link_libraries(helloworld
    VectorUtils3 GL_utilities loadobj 
    ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})

# Copy shaders so executable will find it
# I.e put all shaders in src/shaders folder
add_custom_command(TARGET helloworld POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/src/shaders
    $<TARGET_FILE_DIR:helloworld>/shaders)


