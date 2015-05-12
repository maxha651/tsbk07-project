#include <stdio.h>

#ifdef _WIN32
#include <windows.h>
#endif

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <Camera.h>
#include <ShaderLoader.h>
#include <Game.h>
#include <Input.h>
#include <Context.h>

/*
 * Just put stuff here until we can refactor it into
 * classes. Or not.
 */

static const int UPDATE_TIME_MS = 100;

// Test triangle
static GLfloat vertices[] =
{
    -0.0f,-0.0f,0.0f,
    -0.0f,1.0f,0.0f,
    1.0f,-0.0f,0.0f
};


static std::unique_ptr<Game> game;

static std::unique_ptr<Model> cube;

void init() 
{
	char fragmentShader[128], vertexShader[128];
	sprintf(vertexShader, "%s/VertexShader.glsl", TSBK07_SHADERS_PATH);
	sprintf(fragmentShader, "%s/FragmentShader.glsl", TSBK07_SHADERS_PATH);
	ShaderLoader shaderLoader;
	Context::Instance().program = shaderLoader.CreateProgram(vertexShader, fragmentShader);


    // GL inits
    glClearColor(0.5f,0.5f,0.5f,1.0f);
    glEnable(GL_DEPTH_TEST);

}

void initGameObjects()
{
    game = std::unique_ptr<Game>(new Game(TSBK07_GAMEOBJECTS_PATH));
}

void update(int val)
{
    Input::Update();
    game->Update();
    glutPostRedisplay();
    glutTimerFunc(UPDATE_TIME_MS, update, 0);
}

void display(void)
{
    // clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glClearColor(1.0, 0.5, 0.0, 1.0);//clear red
    //glMatrixMode(GL_MODELVIEW);
    // glLoadIdentity();

	game->Render();

    // Swap buffers
    glutSwapBuffers();
}

void resize(GLsizei w, GLsizei h)
{
    glViewport(0, 0, w, h);
    glutPostRedisplay();
}

void debug(int val) {
    std::cout << "Input: " << Input::GetInputAxis() << std::endl;
    std::cout << "Camera pos: " << Context::Instance().camera->GetTransform()->GetPosition() << std::endl;
    std::cout << "Camera lookDir: " << Context::Instance().camera->lookDir << std::endl;

    glutTimerFunc(1000, debug, 0);
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
    //glutTimerFunc(1000, debug, 0);

    Input::Init();

    glewExperimental = GL_TRUE;
    glewInit();

    init();
    initGameObjects();
    glutMainLoop();

    return 0;
}
