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
#include <Math.h>

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

	//return;
	// -------------- Test Code ----------------------
	
	// LineRenderer
	GameObject &go = game->GetGameObject("linerenderer");
	LineRenderer *l = go.GetComponent<LineRenderer>();

	//spline
	spline.AddSplinePoint(Vector3f(0, 0, -10));
	spline.AddSplinePoint(Vector3f(10, 0, 0));
	spline.AddSplinePoint(Vector3f(0, 0, 10));
	spline.AddSplinePoint(Vector3f(-10, 0, 0));
	spline.AddSplinePoint(Vector3f(0, 0, -10));

	// Create raycast mesh for leftwall and ray cast on it
	GameObject &got = game->GetGameObject("plate");
	Model *plate = got.GetComponent<Model>();
	std::vector<int> patchedIndices;
	for (int i = 0; i < plate->patchedVertices.size(); i++){
		patchedIndices.push_back(i);
	}

	RaycastMesh *rm = createRaycastMesh(plate->patchedVertices.size(), &plate->patchedVertices[0], plate->patchedVertices.size() / 3, (const RmUint32 *)&patchedIndices[0]);
	int counter = 0;

	for (int n = 0; n < plate->patchedVertices.size(); n += 9){
		
		if (plate->patchedVertices[n] >= 5.45f){
			// Raycast
			RmReal to[3] = { (plate->patchedVertices[n] + plate->patchedVertices[n + 3] + plate->patchedVertices[n + 6]) / 3,
				(plate->patchedVertices[n + 1] + plate->patchedVertices[n + 4] + plate->patchedVertices[n + 7]) / 3,
				(plate->patchedVertices[n + 2] + plate->patchedVertices[n + 5] + plate->patchedVertices[n + 8]) / 3 };
			RmReal from[3] = { 8, 5, -2 };
			to[0] = 2*to[0] - from[0];
			to[1] = 2*to[1] - from[1];
			to[2] = 2*to[2] - from[2];
			RmReal hitLocation[3] = { 0, 0, 0 };
			RmReal normal[3];
			RmReal hitDistance;
			int triangleIndex = -1;
			bool hit = rm->raycast(from, to, hitLocation, normal, &hitDistance, (RmUint32*)&triangleIndex);

			if (triangleIndex > -1){
				triangleIndex *= 3;
				Vector3f rayCastLineBackward = Vector3f(from[0] - to[0], from[1] - to[1], from[2] - to[2]);
				Vector3f normals1 = Vector3f(plate->patchedNormals[triangleIndex * 3], plate->patchedNormals[triangleIndex * 3 + 1], plate->patchedNormals[triangleIndex * 3 + 2]);
				float angle = acos(rayCastLineBackward.dot(normals1) / rayCastLineBackward.norm()*normals1.norm());
				float fi1 = rayCastLineBackward.dot(normals1) / rayCastLineBackward.norm()*normals1.norm();
				float deg = angle * 180 / M_PI;
				//std::cout << fi1 << std::endl;

				Vector3f rayCastLine = Vector3f(to[0] - from[0], to[1] - from[1], to[2] - from[2]);
				Vector3f normals2 = Vector3f(-1, 0, 0);
				float angle2 = acos(rayCastLine.dot(normals2) / rayCastLine.norm()*normals2.norm());
				float fi2 = rayCastLine.dot(normals2) / rayCastLine.norm()*normals2.norm();
				float deg2 = angle2 * 180 / M_PI;
				//std::cout << fi2 << std::endl;

				// Calculate radiosity
				Vector3f B = Vector3f(0.7f, 0.7f, 0.7f);
				Vector3f E = Vector3f(0.1f, 0.1f, 0.1f); //Emitted energy
				float p = 3.0f; // Reflectivity
				int vis = 1;

				Vector3f totalEnergy = E + p*B*(1 / (M_PI*hitDistance*hitDistance))*fi1*fi2*vis;
				std::cout << "(1 / M_PI*hitDistance*hitDistance)" << (1 / (M_PI*hitDistance*hitDistance)) << std::endl;
				std::cout << "fi1*fi2 :" << fi1*fi2 << std::endl;
				std::cout << "totalEnergy :" << totalEnergy << std::endl;

				for (int i = 0; i < 12; i += 4){
					plate->colors[triangleIndex * 4 + i] = plate->colors[triangleIndex * 4 + i] + totalEnergy.x();
					plate->colors[triangleIndex * 4 + 1 + i] = plate->colors[triangleIndex * 4 + 1 + i] + totalEnergy.y();
					plate->colors[triangleIndex * 4 + 2 + i] = plate->colors[triangleIndex * 4 + 2 + i] + totalEnergy.z();
				}
				//l->AddLine(Vector3f(from[0], from[1], from[2]), Vector3f(to[0], to[1], to[2]));
			}
		}
	}
	plate->LoadVBOAndVAO();

	// normals
	//l->AddLine(Vector3f(hitLocation[0], hitLocation[1], hitLocation[2]), Vector3f(hitLocation[0] + normals1.x(), hitLocation[1] + normals1.y(), hitLocation[2] + normals1.z()));
	//l->AddLine(Vector3f(from[0], from[1], from[2]), Vector3f(from[0] + normals2.x(), from[1] + normals2.y(), from[2] + normals2.z()));
	/*Vector3f v1;
	Vector3f v2;
	Vector3f v3;
	// Draw lines for all vertices on left wall to see the patches
	GameObject &leftgo = game->GetGameObject("leftwall");
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
