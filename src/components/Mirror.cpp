//
// Created by max on 2015-05-14.
//

#include <Mirror.h>
#include <FBOManager.h>
#include <Context.h>
#include <Game.h>

using Eigen::Vector3f;

static const std::string SHADER = "mirror";
static const std::string TEX_SHADER = "texture";

Mirror::Mirror() : width(0), height(0), normal(1, 0, 0), start(0, 0, 0),
                   left(0, 0, 0), right(0, 0, 0) {

}

Mirror::Mirror(float width, float height, const Eigen::Vector3f& normal,
               const Eigen::Vector3f &start, const Eigen::Vector3f &left,
               Eigen::Vector3f &right) : width(width), height(height),
                normal(normal), start(start), left(left), right(right) {
    this->width = (int) width;
    this->height = (int) height;
    this->normal.normalize();
}

Mirror::Mirror(const std::string& jsonPath) : jsonLoader(jsonPath),
    start(-1, -1, 0), left(-1, 1, 0), right(1, -1, 0) {
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
	//camera.projectionMatrix = Camera::Frustum(-.1f, .1f, -10.0f, 10.0f, 1.0f, 100.0f);

	ShaderLoader shaderLoader;
	std::string shadersPath(TSBK07_SHADERS_PATH);

	program = shaderLoader.CreateProgram((shadersPath + "/" + SHADER + ".vert").c_str(),
										 (shadersPath + "/" + SHADER + ".frag").c_str());
	texProgram = shaderLoader.CreateProgram((shadersPath + "/" + TEX_SHADER + ".vert").c_str(),
										 (shadersPath + "/" + TEX_SHADER + ".frag").c_str());
	LoadVBOAndVAO();
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

	// Change us to renderer NOT USED
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
	/* Render texture to surface program */
	glUseProgram(texProgram);

	/* Change to our context */
	glBindVertexArray(vertexArrayObjID);

	/* TBD I guess */
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, FBOManager::Instance().getColorTexture(fboRef));
	glUniform1i(glGetUniformLocation(texProgram, "texUnit"), GL_TEXTURE0);

	/* Upload matrices */
	glUniformMatrix4fv(glGetUniformLocation(texProgram, "projectionMatrix"), 1, GL_FALSE, Context::Instance().camera->projectionMatrix.data());
	glUniformMatrix4fv(glGetUniformLocation(texProgram, "worldToViewMatrix"), 1, GL_FALSE, Context::Instance().camera->worldToViewMatrix.data());
	glUniformMatrix4fv(glGetUniformLocation(texProgram, "transform"), 1, GL_FALSE, GetTransform()->GetMatrix().data());

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0L);
}

/**
 * Set up our VAO context with vertices, indices buffers and such.
 */
void Mirror::LoadVBOAndVAO() {
	glGenVertexArrays(1, &vertexArrayObjID);
	glBindVertexArray(vertexArrayObjID);

    vertices.insert(vertices.end(), start.data(), start.data() + 3);
    vertices.insert(vertices.end(), left.data(), left.data() + 3);
    vertices.insert(vertices.end(), right.data(), right.data() + 3);
    Vector3f end = -1 * start + left + right;
    vertices.insert(vertices.end(), end.data(), end.data() + 3);
    // Two triangles forming quad
    indices = { 0, 1, 2, 1, 2, 3 };
    // Map to entire surface
    texCoords = { 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f };

	unsigned int vertexBufferObjID;
	glGenBuffers(1, &vertexBufferObjID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(glGetAttribLocation(texProgram, "in_Position"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(texProgram, "in_Position"));


	unsigned int indicesBufferObjID;

	glGenBuffers(1, &indicesBufferObjID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBufferObjID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

	unsigned int normalsBufferObjIDCube;

	glGenBuffers(1, &normalsBufferObjIDCube);
	glBindBuffer(GL_ARRAY_BUFFER, normalsBufferObjIDCube);
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(GLfloat), normal.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(glGetAttribLocation(texProgram, "in_Normal"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(texProgram, "in_Normal"));

	unsigned int texCoordsBufferObjIDCube;
	glGenBuffers(1, &texCoordsBufferObjIDCube);
	glBindBuffer(GL_ARRAY_BUFFER, texCoordsBufferObjIDCube);
	glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(GLfloat), &texCoords[0], GL_STATIC_DRAW);

	glVertexAttribPointer(glGetAttribLocation(texProgram, "in_TexCoord"), 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(texProgram, "in_TexCoord"));
}
