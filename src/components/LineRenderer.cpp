#include "LineRenderer.h"

LineRenderer::LineRenderer()
{
}

LineRenderer::LineRenderer(const std::string &jsonPath) {

}

LineRenderer::~LineRenderer()
{
}

void LineRenderer::SetColor(GLfloat c1, GLfloat c2, GLfloat c3, GLfloat c4) {
	colors[1] = c1;
	colors[2] = c2;
	colors[3] = c3;
	colors[4] = c4;
}

void LineRenderer::LoadVBOAndVAO(){
	glGenVertexArrays(1, &vertexArrayObjID);
	glBindVertexArray(vertexArrayObjID);

	unsigned int vertexBufferObjIDLines;
	glGenBuffers(1, &vertexBufferObjIDLines);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjIDLines);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(glGetAttribLocation(Context::Instance().program, "in_Position"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(Context::Instance().program, "in_Position"));
}

void LineRenderer::Render() {
	BaseComponent::Render();
	// Draw stuff or something
	glBindVertexArray(vertexArrayObjID);	// Select VAO

	glUseProgram(Context::Instance().program);

	// Calculate transform and send to shader

	/*Eigen::Matrix4f matrix = Context::Instance().camera->projectionMatrix *
	Context::Instance().camera->worldToViewMatrix *
	GetGameObject()->transform.GetMatrix();*/

	Camera *c = Context::Instance().camera;
	

	glUniformMatrix4fv(glGetUniformLocation(Context::Instance().program, "projectionMatrix"), 1, GL_FALSE, Context::Instance().camera->projectionMatrix.data());
	glUniformMatrix4fv(glGetUniformLocation(Context::Instance().program, "worldToViewMatrix"), 1, GL_FALSE, Context::Instance().camera->worldToViewMatrix.data());
	glUniformMatrix4fv(glGetUniformLocation(Context::Instance().program, "transform"), 1, GL_FALSE, GetTransform()->GetMatrix().data());
	GLint loc = glGetUniformLocation(Context::Instance().program, "uni_Color");
	glProgramUniform4fv(Context::Instance().program, loc, 1, colors);

	glDrawArrays(GL_LINES, 0, vertices.size() / 3);
}

void LineRenderer::Update() {
	BaseComponent::Update();
}
