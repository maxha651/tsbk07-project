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

	Vector3f v = Vector3f(0,0,-10);
	for (int i = 0; i < 100; i++){
		line_vertexs.push_back(v.x());
		line_vertexs.push_back(v.y());
		line_vertexs.push_back(v.z());
		v = spline.GetInterpolatedSplinePoint(i / 100.0f);
		line_vertexs.push_back(v.x());
		line_vertexs.push_back(v.y());
		line_vertexs.push_back(v.z());
	}


	/*glGenVertexArrays(1, &vertexArrayObjIDLines);
	glBindVertexArray(vertexArrayObjIDLines);

	unsigned int vertexBufferObjIDLines;
	glGenBuffers(1, &vertexBufferObjIDLines);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjIDLines);
	glBufferData(GL_ARRAY_BUFFER, line_vertexs.size() * sizeof(GLfloat), &line_vertexs[0], GL_STATIC_DRAW);

	glVertexAttribPointer(glGetAttribLocation(Context::Instance().program, "in_Position"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(Context::Instance().program, "in_Position"));*/

	GameObject &go = game->GetGameObject("leftwall");
	std::cout << go.GetName() << std::endl;
	Model *m = go.GetComponent<Model>();
	m->SetColor(1, 0, 0, 1);
	std::cout << m->colors[1] << std::endl;
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

	/*Eigen::Matrix4f m;
	m.setIdentity();
	
	glUniformMatrix4fv(glGetUniformLocation(Context::Instance().program, "transform"), 1, GL_FALSE, m.data());
	GLint loc = glGetUniformLocation(Context::Instance().program, "uni_Color");
	glProgramUniform4fv(Context::Instance().program, loc, 1, colors);
	glBindVertexArray(vertexArrayObjIDLines);
	glDrawArrays(GL_LINES, 0, line_vertexs.size()/3);*/


    // Swap buffers
    glutSwapBuffers();

	t = t + 0.01;
	float sinvalue = abs(sin(t));

	//std::cout << spline.GetInterpolatedSplinePoint(sinvalue) << std::endl;


	//Context::Instance().camera->GetTransform()->SetPosition(spline.GetInterpolatedSplinePoint(sinvalue));
	//Context::Instance().camera->lookDir = Vector3f(0,0,0);
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
