#pragma once

// Third party
#include <Eigen/Geometry>
#include <GL/glew.h>
#include <GL/freeglut.h>

// Ours
#include <BaseComponent.h>
#include <Context.h>
#include <Camera.h>

class LineRenderer : public BaseComponent
{
public:
	LineRenderer();
	LineRenderer(GameObject* gameObject, const std::string& jsonPath);
	~LineRenderer();
	
	GLuint vertexArrayObjID;
	std::vector<GLfloat> vertices;
	GLfloat colors[4];

	void AddLine(Vector3f from, Vector3f to);
	void SetColor(GLfloat c1, GLfloat c2, GLfloat c3, GLfloat c4);
	virtual void Update() override;
	virtual void Render() override;
	virtual void Start() override;

private:
	void LoadVBOAndVAO();
};

