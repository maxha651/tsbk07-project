#include <stdio.h>

#ifdef _WIN32
#include <windows.h>
#endif

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <Camera.h>

#include <ShaderLoader.h>
#include <Game.h>
#include <ModelAndShader.h>


/*
 * Just put stuff here until we can refactor it into
 * classes. Or not.
 */

static const int UPDATE_TIME_MS = 20;

// Test triangle
static GLfloat vertices[] =
{
    -0.0f,-0.0f,0.0f,
    -0.0f,1.0f,0.0f,
    1.0f,-0.0f,0.0f
};


static GLuint vertexArrayObjIDTriangle;
static GLuint vertexArrayObjIDCube;
static GLuint program;
static Camera camera;
static Game *game = nullptr;


static ModelAndShader cube = ModelAndShader("movedcube", "none");

void init() 
{
    
    // GL inits
    glClearColor(1.0f,0.3f,0.5f,0.0f);
    glEnable(GL_DEPTH_TEST);

    // Load and compile shader
	char fragmentShader[128], vertexShader[128];
	sprintf(vertexShader, "%s/VertexShader.glsl", TSBK07_SHADERS_PATH);
    sprintf(fragmentShader, "%s/FragmentShader.glsl", TSBK07_SHADERS_PATH);
    ShaderLoader shaderLoader;
    program = shaderLoader.CreateProgram(vertexShader, fragmentShader);
    glUseProgram(program);

    // Triangle
	glGenVertexArrays(1, &vertexArrayObjIDTriangle);
	glBindVertexArray(vertexArrayObjIDTriangle);

	unsigned int vertexBufferObjIDTriangle;
	glGenBuffers(1, &vertexBufferObjIDTriangle);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjIDTriangle);
    glBufferData(GL_ARRAY_BUFFER, 9*sizeof(GLfloat), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(glGetAttribLocation(program, "in_Position"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(program, "in_Position"));


	// Cube
	glGenVertexArrays(1, &vertexArrayObjIDCube);
	glBindVertexArray(vertexArrayObjIDCube);

	unsigned int vertexBufferObjIDCube;
	glGenBuffers(1, &vertexBufferObjIDCube);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjIDCube);
	glBufferData(GL_ARRAY_BUFFER, cube.vertices.size() * sizeof(GLfloat), &cube.vertices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(glGetAttribLocation(program, "in_Position"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(program, "in_Position"));

	unsigned int normalsBufferObjIDCube;
	glGenBuffers(1, &normalsBufferObjIDCube);
	glBindBuffer(GL_ARRAY_BUFFER, normalsBufferObjIDCube);
	glBufferData(GL_ARRAY_BUFFER, cube.normals.size() * sizeof(GLfloat), &cube.normals[0], GL_STATIC_DRAW);

	glVertexAttribPointer(glGetAttribLocation(program, "in_Normal"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(program, "in_Normal"));

}

void initGameObjects()
{
    game = new Game(TSBK07_GAMEOBJECTS_PATH);
}

void keyboard(unsigned char key, int x, int y)
{

}

void update(int val)
{
    game->Update();
    glutTimerFunc(UPDATE_TIME_MS, update, 0);
}

void render()
{
    game->Render();
}

void display(void)
{

    // clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(1.0, 0.0, 0.0, 1.0);//clear red
    // glMatrixMode(GL_MODELVIEW);
    // glLoadIdentity();

	glBindVertexArray(vertexArrayObjIDTriangle);	// Select VAO for triangle
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glBindVertexArray(vertexArrayObjIDCube);	// Select VAO for cube
	glDrawArrays(GL_TRIANGLES, 0, cube.vertices.size()/3);

    // Swap buffers
    glutSwapBuffers();
}

void resize(GLsizei w, GLsizei h)
{
    glViewport(0, 0, w, h);
    glutPostRedisplay();
}

void cleanup()
{
    if (game != nullptr) {
        delete game;
    }
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
    glutTimerFunc(UPDATE_TIME_MS, update, 0);

    glutKeyboardFunc(keyboard);
    glewExperimental = GL_TRUE;
    glewInit();

    init();
    initGameObjects();
    glutMainLoop();

    cleanup();

    return 0;
}
