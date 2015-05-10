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

using Eigen::Vector3f;
using Eigen::Vector2f;

// Ours
#include <JSONLoader.h>
#include <BaseComponent.h>
#include <ShaderLoader.h>

/**
* Model and shader component.
*/
class ModelAndShader : public BaseComponent
{
public:
	ModelAndShader(const std::string& jsonPath);
	ModelAndShader(const char *model, const char *vertexshader, const char *fragmentshader);
	~ModelAndShader();
	std::vector<GLfloat> vertices;
	std::vector<GLfloat> normals;
	GLuint program;
	GLuint vertexArrayObjID;

	virtual void Update() override;
	virtual void Render() override;


private:
    // DAVID: As it is now, these will be monitored by JSONLoader
    // Changes will be written back to JSON file on destruct
    std::string model, vertshader, fragshader;
	// Keep all dependent variables before this
	JSONLoader jsonLoader;

	

	void init(const char *model, const char *vertexshader, const char *fragmentshader);
	void LoadObject(const char* filename);
	void LoadShader(const char *vertShader, const char *fragShader);
	void LoadVBOAndVAO();
};

