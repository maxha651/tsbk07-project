#include <stdio.h>

#ifdef _WIN32
  #include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/freeglut.h>

#include <GL_utilities.h>
#include <VectorUtils3.h>
#include <loadobj.h>


GLfloat vertices[] =
{
    -0.0f,-0.0f,0.0f,
    -0.0f,1.0f,0.0f,
    1.0f,-0.0f,0.0f
};

unsigned int vertexArrayObjID;
GLuint program;

void init() 
{
    unsigned int vertexBufferObjID;
    // Reference to shader program
    GLuint program;
    // GL inits
    glClearColor(1.0,0.3,0.5,0);
    glDisable(GL_DEPTH_TEST);
    // Load and compile shader
    program = loadShaders("shaders/lab1-1.vert", "shaders/lab1-1.frag");
    // Allocate and activate Vertex Array Object
    glGenVertexArrays(1, &vertexArrayObjID);
    glBindVertexArray(vertexArrayObjID);
    // Allocate Vertex Buffer Objects
    glGenBuffers(1, &vertexBufferObjID);
    // VBO for vertex data
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjID);
    glBufferData(GL_ARRAY_BUFFER, 9*sizeof(GLfloat), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(glGetAttribLocation(program, "in_Position"), 3, GL_FLOAT, GL_FALSE, 0, 0); 
    glEnableVertexAttribArray(glGetAttribLocation(program, "in_Position"));
}

void display(void)
{
    // clear the screen
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glBindVertexArray(vertexArrayObjID);	// Select VAO
    glDrawArrays(GL_TRIANGLES, 0, 3);	// draw object

    // Swap buffers
    glutSwapBuffers();
}

void resize(GLsizei w, GLsizei h)
{
    glViewport(0, 0, w, h);
    glutPostRedisplay();
}

int main(int argc, char *argv[])
{
    // Using OpenGL version 3.2
    glutInitContextVersion(3,2);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA|GLUT_SINGLE);
    glutInitWindowPosition(50, 25);
    glutInitWindowSize(480,480);
    glutCreateWindow("Hello OpenGL");
    glutDisplayFunc(display);
    init();
    glutMainLoop();

    return 0;
}
