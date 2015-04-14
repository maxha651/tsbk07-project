#include <stdio.h>

#ifdef _WIN32
#include <windows.h>
#endif

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <Camera.h>

#include <ShaderLoader.h>

static GLfloat vertices[] =
{
    -0.0f,-0.0f,0.0f,
    -0.0f,1.0f,0.0f,
    1.0f,-0.0f,0.0f
};

static GLuint vertexArrayObjID;
static GLuint program;
static Camera camera;

void init() 
{

    unsigned int vertexBufferObjID;
    // GL inits
    glClearColor(1.0,0.3,0.5,0);
    glDisable(GL_DEPTH_TEST);
    // Load and compile shader
    //program = loadShaders("shaders/lab1-1.vert", "shaders/lab1-1.frag");
    ShaderLoader shaderLoader;
    program = shaderLoader.CreateProgram("shaders/VertexShader.glsl",
            "shaders/FragmentShader.glsl");
    glUseProgram(program);
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

void keyboard(unsigned char key, int x, int y)
{

}

void display(void)
{
    // clear the screen
    //glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(1.0, 0.0, 0.0, 1.0);//clear red
    // glMatrixMode(GL_MODELVIEW);
    // glLoadIdentity();

    //glBindVertexArray(vertexArrayObjID);	// Select VAO

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
    glutKeyboardFunc(keyboard);
    glewExperimental = GL_TRUE;
    glewInit();
    init();
    glutMainLoop();

    return 0;
}
