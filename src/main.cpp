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
#include <RaycastMesh.h>

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

GLuint vertexArrayObjIDLines;

void initGameObjects()
{
    game = std::unique_ptr<Game>(new Game(TSBK07_GAMEOBJECTS_PATH));
	game->Start();

	// -------------- Test Code ----------------------
	
	//spline
	spline.AddSplinePoint(Vector3f(0, 0, -10));
	spline.AddSplinePoint(Vector3f(10, 0, 0));
	spline.AddSplinePoint(Vector3f(0, 0, 10));
	spline.AddSplinePoint(Vector3f(-10, 0, 0));
	spline.AddSplinePoint(Vector3f(0, 0, -10));

	// Create raycast mesh for leftwall and ray cast on it
	GameObject &got = game->GetGameObject("leftwall");
	Model *plate = got.GetComponent<Model>();
	std::vector<int> patchedIndices;
	for (int i = 0; i < plate->patchedVertices.size(); i++){
		patchedIndices.push_back(i);
	}

	RaycastMesh *rm = createRaycastMesh(plate->patchedVertices.size(), &plate->patchedVertices[0], plate->patchedVertices.size() / 3, (const RmUint32 *)&patchedIndices[0]);
	// Raycast
	RmReal to[3] = {0, 0, 0 };
	RmReal from[3] = { 10, 5, -2 };
	RmReal hitLocation[3] = { 0, 0, 0 };
	RmReal normal[3];
	RmReal hitDistance;
	RmUint32 triangleIndex = -1;
	bool hit = rm->raycast(from, to, hitLocation, normal, &hitDistance, &triangleIndex);

	// color the triangle we hit with red
	plate->colors[triangleIndex * 4] = 1;
	plate->colors[triangleIndex * 4 + 1] = 0;
	plate->colors[triangleIndex * 4 + 4] = 1;
	plate->colors[triangleIndex * 4 + 5] = 0;
	plate->colors[triangleIndex * 4 + 8] = 1;
	plate->colors[triangleIndex * 4 + 9] = 0;
	plate->LoadVBOAndVAO();

	// Draw line for the raycast
	GameObject &go = game->GetGameObject("linerenderer");
	LineRenderer *l = go.GetComponent<LineRenderer>();
	Vector3f v1;
	Vector3f v2;
	Vector3f v3;
	l->AddLine(Vector3f(from[0], from[1], from[2]), Vector3f(to[0], to[1], to[2]));

	// Draw lines for all vertices on left wall to see the patches
	/*GameObject &leftgo = game->GetGameObject("leftwall");
	Model *leftwall = leftgo.GetComponent<Model>();

	for (int n = 0; n < leftwall->patchedVertices.size(); n += 9){
		v1 = Vector3f(leftwall->patchedVertices[n], leftwall->patchedVertices[n + 1], leftwall->patchedVertices[n + 2]);
		v2 = Vector3f(leftwall->patchedVertices[n + 3], leftwall->patchedVertices[n + 4], leftwall->patchedVertices[n + 5]);
		v3 = Vector3f(leftwall->patchedVertices[n + 6], leftwall->patchedVertices[n + 7], leftwall->patchedVertices[n + 8]);
		l->AddLine(v1, v2);
		l->AddLine(v2, v3);
		l->AddLine(v1, v3);
	}*/
	

	/*Vector3f oldV = Vector3f(0,0,-10);
	Vector3f v = Vector3f(0, 0, -10);
	for (int i = 50; i < 100; i++){

		v = spline.GetInterpolatedSplinePoint(i / 99.0f);
		l->AddLine(oldV, v);
		oldV = v;
	}*/

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
