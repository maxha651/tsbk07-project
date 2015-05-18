#pragma once

//
// Created by David on 2015-04-14.
//

// C++ std lib
#include <cstdlib>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <cstdio>
#include <iostream>

// Third party
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <Eigen/Geometry>

using Eigen::Vector4f;
using Eigen::Vector3f;
using Eigen::Vector2f;
using Eigen::Matrix3f;
using Eigen::Matrix4f;

// Ours
#include <JSONLoader.h>
#include <BaseComponent.h>
#include <ShaderLoader.h>
#include <Context.h>

/**
* Model and shader component.
*/
class Model : public BaseComponent
{
public:
	Model(const std::string& jsonPath);
	Model(const char *model);
	~Model();
	std::vector<GLfloat> patchedVertices;
	std::vector<GLfloat> patchedNormals;
	std::vector<GLfloat> vertices;
	std::vector<GLfloat> normals;
	GLfloat colors[4];
	GLuint vertexArrayObjID;

	virtual void Update() override;
	virtual void Render() override;
	virtual void Start() override;
	void SetColor(GLfloat c1, GLfloat c2, GLfloat c3, GLfloat c4);


private:
    // DAVID: As it is now, these will be monitored by JSONLoader
    // Changes will be written back to JSON file on destruct
    std::string model, vertshader, fragshader;
	// Keep all dependent variables before this
	JSONLoader jsonLoader;
	
	float MIN_PATCH_AREA = 2.0f;

	void Model::AddTriangle(std::vector<GLfloat> *ver, Vector3f vec1, Vector3f vec2, Vector3f vec3);
	void SplitTriangles();
	void init(const char *model);
	void LoadObject(const char* filename);
	void LoadVBOAndVAO();
};

