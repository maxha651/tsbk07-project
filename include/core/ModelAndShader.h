//
// Created by David on 2015-04-14.
//


#include <Component.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include <cstdlib>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <cstdio>
#include <iostream>

#pragma once

/**
* Model and shader component.
*/
class ModelAndShader : public Component
{
public:
	ModelAndShader(const char *model, const char *shader);
	~ModelAndShader();
	int id;

private:
        struct face;
        struct coordinate;
	int LoadObject(const char* filename);
	GLfloat* vertices;
};

