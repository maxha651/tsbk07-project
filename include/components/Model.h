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
	std::vector<GLfloat> vertices;
	std::vector<GLfloat> normals;
	std::vector<GLfloat> color;
	GLuint vertexArrayObjID;

	virtual void Update() override;
	virtual void Render() override;


private:
    // DAVID: As it is now, these will be monitored by JSONLoader
    // Changes will be written back to JSON file on destruct
    std::string model, vertshader, fragshader;
	// Keep all dependent variables before this
	JSONLoader jsonLoader;

	

	void init(const char *model);
	void LoadObject(const char* filename);
	void LoadVBOAndVAO();
};

