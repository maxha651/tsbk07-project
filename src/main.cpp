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
#include <math.h>
#include <Radiosity.h>
#include <Model.h>

/*
 * Just put stuff here until we can refactor it into
 * classes. Or not.
 */

static const int UPDATE_TIME_MS = 30; // > 60 fps

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
	Context::Instance().preBaked = false;

    // GL inits
    glClearColor(0.5f,0.5f,0.5f,1.0f);
    glEnable(GL_DEPTH_TEST);
}


std::vector<GLfloat> line_vertexs;

GLuint vertexArrayObjIDLines;

Radiosity radiosity;
bool useCameraSpline = true;

void initGameObjects()
{
    game = std::unique_ptr<Game>(new Game(TSBK07_GAMEOBJECTS_PATH));
	game->Awake();

	if (!Context::Instance().preBaked) {
		// Create raycast mesh for leftwall and ray cast on it
		GameObject &goleft = game->GetGameObject("leftwall");
		Model *plate = goleft.GetComponent<Model>();
		GameObject &golamp = game->GetGameObject("lamp");
		Model *lamp = golamp.GetComponent<Model>();
		GameObject &goright = game->GetGameObject("rightwall");
		Model *right = goright.GetComponent<Model>();
		GameObject &goback = game->GetGameObject("backwall");
		Model *back = goback.GetComponent<Model>();
		GameObject &floor = game->GetGameObject("floor");
		Model *floo = floor.GetComponent<Model>();
		GameObject &gocube = game->GetGameObject("cube");
		Model *cube = gocube.GetComponent<Model>();
		/*GameObject &gocube2 = game->GetGameObject("cube2");
		Model *cube2 = gocube2.GetComponent<Model>();*/
		radiosity.AddPatches(right->patches);
		radiosity.AddPatches(plate->patches);
		radiosity.AddPatches(lamp->patches);
		radiosity.AddPatches(back->patches);
		radiosity.AddPatches(floo->patches);
		radiosity.AddPatches(cube->patches);
		//radiosity.AddPatches(cube2->patches);

		radiosity.CreateRayCastMesh();
		radiosity.CalculateRadiosity();
	}

	game->Start();

	//return;
	// -------------- Test Code ----------------------
	
	

	// LineRenderer
	GameObject &go = game->GetGameObject("linerenderer");
	LineRenderer *l = go.GetComponent<LineRenderer>();
	/*

	
	Vector3f v1;
	Vector3f v2;
	Vector3f v3;

	GameObject &goleft = game->GetGameObject("leftwall");
	Model *leftwall = goleft.GetComponent<Model>();
	for (int n = 0; n < leftwall->patchedVertices.size(); n += 9){
		v1 = Vector3f(leftwall->patchedVertices[n], leftwall->patchedVertices[n + 1], leftwall->patchedVertices[n + 2]);
		v2 = Vector3f(leftwall->patchedVertices[n + 3], leftwall->patchedVertices[n + 4], leftwall->patchedVertices[n + 5]);
		v3 = Vector3f(leftwall->patchedVertices[n + 6], leftwall->patchedVertices[n + 7], leftwall->patchedVertices[n + 8]);
		l->AddLine(v1, v2);
		l->AddLine(v2, v3);
		l->AddLine(v1, v3);
	}

	GameObject &platego = game->GetGameObject("rightwall");
	Model *platecom = platego.GetComponent<Model>();

	for (int n = 0; n < platecom->patchedVertices.size(); n += 9){
		v1 = Vector3f(platecom->patchedVertices[n], platecom->patchedVertices[n + 1], platecom->patchedVertices[n + 2]);
		v2 = Vector3f(platecom->patchedVertices[n + 3], platecom->patchedVertices[n + 4], platecom->patchedVertices[n + 5]);
		v3 = Vector3f(platecom->patchedVertices[n + 6], platecom->patchedVertices[n + 7], platecom->patchedVertices[n + 8]);
		l->AddLine(v1, v2);
		l->AddLine(v2, v3);
		l->AddLine(v1, v3);
	}

	GameObject &goback = game->GetGameObject("backwall");
	Model *backwall = goback.GetComponent<Model>();

	for (int n = 0; n < backwall->patchedVertices.size(); n += 9){
		v1 = Vector3f(backwall->patchedVertices[n], backwall->patchedVertices[n + 1], backwall->patchedVertices[n + 2]);
		v2 = Vector3f(backwall->patchedVertices[n + 3], backwall->patchedVertices[n + 4], backwall->patchedVertices[n + 5]);
		v3 = Vector3f(backwall->patchedVertices[n + 6], backwall->patchedVertices[n + 7], backwall->patchedVertices[n + 8]);
		l->AddLine(v1, v2);
		l->AddLine(v2, v3);
		l->AddLine(v1, v3);
	}

	GameObject &gofloor = game->GetGameObject("floor");
	Model *floor = gofloor.GetComponent<Model>();

	for (int n = 0; n < floor->patchedVertices.size(); n += 9){
		v1 = Vector3f(floor->patchedVertices[n], floor->patchedVertices[n + 1], floor->patchedVertices[n + 2]);
		v2 = Vector3f(floor->patchedVertices[n + 3], floor->patchedVertices[n + 4], floor->patchedVertices[n + 5]);
		v3 = Vector3f(floor->patchedVertices[n + 6], floor->patchedVertices[n + 7], floor->patchedVertices[n + 8]);
		l->AddLine(v1, v2);
		l->AddLine(v2, v3);
		l->AddLine(v1, v3);
	}
	GameObject &gocube = game->GetGameObject("cube");
	Model *cube = gocube.GetComponent<Model>();

	for (int n = 0; n < cube->patchedVertices.size(); n += 9){
		v1 = Vector3f(cube->patchedVertices[n], cube->patchedVertices[n + 1], cube->patchedVertices[n + 2]);
		v2 = Vector3f(cube->patchedVertices[n + 3], cube->patchedVertices[n + 4], cube->patchedVertices[n + 5]);
		v3 = Vector3f(cube->patchedVertices[n + 6], cube->patchedVertices[n + 7], cube->patchedVertices[n + 8]);
		l->AddLine(v1, v2);
		l->AddLine(v2, v3);
		l->AddLine(v1, v3);
	}*/
	
	if (useCameraSpline){
		//spline
		spline.AddSplinePoint(Vector3f(0, 0, -15));
		spline.AddSplinePoint(Vector3f(0, 0, -6));
		spline.AddSplinePoint(Vector3f(-6, 10, 0));
		spline.AddSplinePoint(Vector3f(0, 10, 6));
		spline.AddSplinePoint(Vector3f(6, 10, 0));
		spline.AddSplinePoint(Vector3f(0, 5, -8));
		spline.AddSplinePoint(Vector3f(0, 0, -5));
		spline.AddSplinePoint(Vector3f(5, 0, 0));
		spline.AddSplinePoint(Vector3f(0, 0, 5));
		spline.AddSplinePoint(Vector3f(-5, 0, 0));
		spline.AddSplinePoint(Vector3f(0, 0, -5));
		spline.AddSplinePoint(Vector3f(0, 0, -15));

		Vector3f oldV = spline.GetInterpolatedSplinePoint(0);
		Vector3f v;
		for (int i = 1; i < 100; i++){

			v = spline.GetInterpolatedSplinePoint((float)i / 100.0f);
			l->AddLine(oldV, v);
			oldV = v;
		}
	}

}

int t = 0;

void CalculateCameraSpline(){
	if (useCameraSpline){
		Vector3f pos = spline.GetInterpolatedSplinePoint((t % 800) / 800.0f);
		Vector3f target = Vector3f(0, 0, 0);
		Vector3f up = Vector3f(0, 1, 0);
		Vector3f lookDir = target - pos;
		Context::Instance().camera->lookDir = lookDir;
		Context::Instance().camera->GetTransform()->SetPosition(pos);
		t += 1;
	}
}

void update(int val)
{
    Input::Update();
    if (Input::IsKeyDown('o')) {
        useCameraSpline = !useCameraSpline;
    }
    game->Update();
    glutPostRedisplay();
    glutTimerFunc(UPDATE_TIME_MS, update, 0);

    CalculateCameraSpline();
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	game->Render();

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
