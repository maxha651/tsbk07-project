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
#include <Component.h>
#include <JSONLoader.h>

/**
* Model and shader component.
*/
class ModelAndShader : public Component
{
public:
	ModelAndShader(const std::string& jsonPath);
	ModelAndShader(const char *model, const char *shader);
	~ModelAndShader();
	std::vector<GLfloat> vertices;
	std::vector<GLfloat> normals;

private:
    JSONLoader jsonLoader;
    // DAVID: As it is now, these will be monitored by JSONLoader
    // Changes will be written back to JSON file on destruct
    std::string model, shader;

    void init(const char *model, const char *shader);
	void LoadObject(const char* filename);
};

