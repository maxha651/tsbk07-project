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
#include <CatSpline.h>
#include <Context.h>
#include <LineRenderer.h>

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

static CatSpline spline;




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


std::vector<GLfloat> line_vertexs;


float colors[] =
{
	1, 0, 0, 1
};

GLuint vertexArrayObjIDLines;

void initGameObjects()
{
    game = std::unique_ptr<Game>(new Game(TSBK07_GAMEOBJECTS_PATH));

	spline.AddSplinePoint(Vector3f(0, 0, -10));
	spline.AddSplinePoint(Vector3f(10, 0, 0));
	spline.AddSplinePoint(Vector3f(0, 0, 10));
	spline.AddSplinePoint(Vector3f(-10, 0, 0));
	spline.AddSplinePoint(Vector3f(0, 0, -10));

	GameObject &go = game->GetGameObject("linerenderer");
	std::cout << go.GetName() << std::endl;
	LineRenderer *l = go.GetComponent<LineRenderer>();
	/*Vector3f from = Vector3f(0, 0, 0);
	Vector3f to = Vector3f(10, 10, 10);
	l->AddLine(from, to);
	from = Vector3f(10, 10, 10);
	to = Vector3f(10, 5, 1);
	l->AddLine(to, from);
	from = Vector3f(10, 5, 1);
	to = Vector3f(0, 0, 0);
	l->AddLine(to, from);*/

	Vector3f oldV = Vector3f(0,0,-10);
	Vector3f v = Vector3f(0, 0, -10);
	for (int i = 0; i < 100; i++){


		v = spline.GetInterpolatedSplinePoint(i / 99.0f);
		l->AddLine(oldV, v);
		oldV = v;
	}


}

void update(int val)
{
    Input::Update();
    game->Update();
    glutPostRedisplay();
    glutTimerFunc(UPDATE_TIME_MS, update, 0);
}

float t = 0;



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
    glutInitDisplayMode(GLUT_DEPTH|GLUT_RGBA|GLUT_DOUBLE);
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
