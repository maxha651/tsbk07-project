cmake_minimum_required (VERSION 2.8)

add_definitions(-DGL_GLEXT_PROTOTYPES)

project ("tsbk07-project")

# Add OpenGL and GLUT
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )

include_directories(include)

# Set up libraries which can be linked with executable
add_library(VectorUtils3 src/utils/VectorUtils3.c)
add_library(GL_utilities src/utils/GL_utilities.c)
add_library(loadobj src/utils/loadobj.c)

add_executable(test src/main.cpp)

target_link_libraries(test 
    VectorUtils3 GL_utilities loadobj 
    ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})

# Copy shaders so executable will find it
# I.e put all shaders in src/shaders folder
add_custom_command(TARGET test POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/src/shaders
    $<TARGET_FILE_DIR:test>/shaders)


