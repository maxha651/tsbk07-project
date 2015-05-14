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
	~LineRenderer();
	
	GLuint vertexArrayObjID;
	std::vector<GLfloat> vertices;
	GLfloat colors[4];

	void SetColor(GLfloat c1, GLfloat c2, GLfloat c3, GLfloat c4);
	virtual void Update() override;
	virtual void Render() override;

private:
	void LoadVBOAndVAO();
};

