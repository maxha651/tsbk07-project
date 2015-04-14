//
// Created by David on 2015-04-14.
//

#include <Component.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#pragma once

/**
* Model and shader component.
*/
class ModelAndShader : public Component
{
public:
	ModelAndShader(const char *model, const char *shader);
	~ModelAndShader();

private:
	GLfloat* vertices;
};

