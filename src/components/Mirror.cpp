//
// Created by max on 2015-05-14.
//

#include <Mirror.h>
#include <FBOManager.h>
#include <Context.h>
#include <Game.h>

static const std::string SHADER = "mirror";
static const std::string TEX_SHADER = "texture";

static GLfloat vertices[] =
{
    0.0f,0.0f,0.0f,
    0.0f,10.0f,0.0f,
    10.0f,0.0f,0.0f,
	10.0f,10.0f,0.0f,
};
static GLuint indices[] =
{
	0, 1, 2, 1, 2, 3
};

static GLfloat texCoords[] =
{
    0.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 0.0f,
	1.0f, 1.0f,
};

Mirror::Mirror() : width(0), height(0), normal(1, 0, 0) {

}

Mirror::Mirror(const std::string& jsonPath) : jsonLoader(jsonPath) {
    jsonLoader.AddDataField("width", &width);
    jsonLoader.AddDataField("height", &height);
    jsonLoader.AddArrayField("normal", normal.data(), 3);
    jsonLoader.Read();

    // no fractal part
    width = (int) width;
    height = (int) height;

    normal.normalize();
}

void Mirror::Init() {
    static unsigned int rendererRef = 0;
    this->rendererRef = ++rendererRef;
    fboRef = FBOManager::Instance().AddFBO((unsigned int) width, (unsigned int) height);
    camera.SetGameObject(GetGameObject());
    camera.lookDir = normal;
	camera.projectionMatrix = Camera::Frustum(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);

	ShaderLoader shaderLoader;
	std::string shadersPath(TSBK07_SHADERS_PATH);

	program = shaderLoader.CreateProgram((shadersPath + "/" + SHADER + ".vert").c_str(),
										 (shadersPath + "/" + SHADER + ".frag").c_str());
	texProgram = shaderLoader.CreateProgram((shadersPath + "/" + TEX_SHADER + ".vert").c_str(),
										 (shadersPath + "/" + TEX_SHADER + ".frag").c_str());
}

void Mirror::Render() {
    BaseComponent::Render();
	RenderTexture();
}

void Mirror::Start() {
	BaseComponent::Start();
	Init();
	camera.Start();
}

void Mirror::Update() {
	BaseComponent::Update();
	camera.Update();

	// Change us to renderer
	unsigned int oldRenderer = Context::Instance().renderer;
	Context::Instance().renderer = rendererRef;
	// Change to our camera
	Camera* oldCamera = Context::Instance().camera;
	Context::Instance().camera = &camera;
	// Change to special shaders
	GLuint oldProgram = Context::Instance().program;
	Context::Instance().program = program;

	// Change target to our FBO
	FBOManager::Instance().ActivateFBO(fboRef);

	// Render to texture
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Context::Instance().game->Render();

	// Reset
	Context::Instance().program = oldProgram;
	Context::Instance().camera = oldCamera;
	Context::Instance().renderer = oldRenderer;
	FBOManager::Instance().DeactivateFBO();
}

void Mirror::RenderTexture() {
	glUseProgram(texProgram);

	LoadVBOAndVAO();

	glUniformMatrix4fv(glGetUniformLocation(texProgram, "projectionMatrix"), 1, GL_FALSE, Context::Instance().camera->projectionMatrix.data());
	glUniformMatrix4fv(glGetUniformLocation(texProgram, "worldToViewMatrix"), 1, GL_FALSE, Context::Instance().camera->worldToViewMatrix.data());
	glUniformMatrix4fv(glGetUniformLocation(texProgram, "transform"), 1, GL_FALSE, GetTransform()->GetMatrix().data());

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0L);
}

void Mirror::LoadVBOAndVAO(){
	glGenVertexArrays(1, &vertexArrayObjID);
	glBindVertexArray(vertexArrayObjID);

	unsigned int vertexBufferObjID;
	glGenBuffers(1, &vertexBufferObjID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(glGetAttribLocation(texProgram, "in_Position"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(texProgram, "in_Position"));


	unsigned int indicesBufferObjID;

	glGenBuffers(1, &indicesBufferObjID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBufferObjID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	unsigned int normalsBufferObjIDCube;

	glGenBuffers(1, &normalsBufferObjIDCube);
	glBindBuffer(GL_ARRAY_BUFFER, normalsBufferObjIDCube);
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(GLfloat), normal.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(glGetAttribLocation(texProgram, "in_Normal"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(texProgram, "in_Normal"));

	unsigned int texCoordsBufferObjIDCube;
	glGenBuffers(1, &texCoordsBufferObjIDCube);
	glBindBuffer(GL_ARRAY_BUFFER, texCoordsBufferObjIDCube);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_STATIC_DRAW);

	glVertexAttribPointer(glGetAttribLocation(texProgram, "in_TexCoord"), 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(texProgram, "in_TexCoord"));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, GL_TEXTURE0);//FBOManager::Instance().getColorTexture(fboRef));
	glUniform1i(glGetUniformLocation(texProgram, "texUnit"), FBOManager::Instance().getColorTexture(fboRef));
}
