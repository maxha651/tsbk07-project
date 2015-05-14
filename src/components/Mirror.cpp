//
// Created by max on 2015-05-14.
//

#include <Mirror.h>
#include <FBOManager.h>
#include <Context.h>
#include <Game.h>

static const unsigned int REF_PREFIX = 0x0100;// Test triangle

static GLfloat vertices[] =
{
    -0.0f,-0.0f,0.0f,
    -0.0f,1.0f,0.0f,
    1.0f,-0.0f,0.0f
};

Mirror::Mirror() : width(0), height(0), normal(1, 0, 0) {

}

Mirror::Mirror(const std::string& jsonPath) : jsonLoader(jsonPath) {
    jsonLoader.AddDataField<float>("width", &width);
    jsonLoader.AddDataField<float>("height", &height);
    jsonLoader.AddArrayField<float>("normal", normal.data(), 3);
    jsonLoader.Read();

    // no fractal part
    width = (int) width;
    height = (int) height;

    normal.normalize();
}

void Mirror::Init() {
    static unsigned int rendererRef = 0;
    this->rendererRef = rendererRef++;
    fboRef = FBOManager::Instance().AddFBO((unsigned int) width, (unsigned int) height);
    camera.SetGameObject(GetGameObject());
    camera.lookDir = normal;
}

void Mirror::Update() {
    BaseComponent::Update();
}

void Mirror::Render() {
    if (Context::Instance().renderer == rendererRef) {
        // To prevent recursion
        return;
    }
    BaseComponent::Render();

    // Change us to renderer
    unsigned int oldRenderer = Context::Instance().renderer;
    Context::Instance().renderer = rendererRef;
    // Change to our camera
    Camera* oldCamera = Context::Instance().camera;
    Context::Instance().camera = &camera;
    // Change target to our FBO
    FBOManager::Instance().ActivateFBO(fboRef);

    // Change to special shaders TODO ???

    // Render to texture
    Context::Instance().game->Render();

    // Reset
    Context::Instance().camera = oldCamera;
    Context::Instance().renderer = oldRenderer;
    // Render to screen
    FBOManager::Instance().ActivateFBO(0);

    RenderTexture();
}

void Mirror::RenderTexture() {
	glUseProgram(Context::Instance().program);
	LoadVBOAndVAO();

	Eigen::Matrix4f matrix = Context::Instance().camera->projectionMatrix *
							Context::Instance().camera->worldToViewMatrix *
							 GetGameObject()->transform.GetMatrix();

	glUniformMatrix4fv(glGetUniformLocation(Context::Instance().program, "projectionMatrix"), 1, GL_FALSE, Context::Instance().camera->projectionMatrix.data());
	glUniformMatrix4fv(glGetUniformLocation(Context::Instance().program, "worldToViewMatrix"), 1, GL_FALSE, Context::Instance().camera->worldToViewMatrix.data());
	glUniformMatrix4fv(glGetUniformLocation(Context::Instance().program, "transform"), 1, GL_FALSE, GetTransform()->GetMatrix().data());

	glDrawArrays(GL_QUADS, 0, 4);
}

void Mirror::LoadVBOAndVAO(){
	unsigned int program = Context::Instance().program;

    unsigned int vertexArrayObjID;
	glGenVertexArrays(1, &vertexArrayObjID);
	glBindVertexArray(vertexArrayObjID);

	unsigned int vertexBufferObjID;
	glGenBuffers(1, &vertexBufferObjID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(glGetAttribLocation(program, "in_Position"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(program, "in_Position"));

	unsigned int normalsBufferObjIDCube;
	glGenBuffers(1, &normalsBufferObjIDCube);
	glBindBuffer(GL_ARRAY_BUFFER, normalsBufferObjIDCube);
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(GLfloat), normal.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(glGetAttribLocation(program, "in_Normal"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(program, "in_Normal"));

}
