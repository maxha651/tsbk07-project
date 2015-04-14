//
// Created by David on 2015-04-13.
//
//From http://in2gpu.com/2014/10/29/shaders-basics/

#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>

class ShaderLoader
{
	private:

		std::string ReadShader(char *filename);
		GLuint CreateShader(GLenum shaderType,
							std::string source,
							char* shaderName);

	public:

		ShaderLoader();
		~ShaderLoader();
		GLuint CreateProgram(char* VertexShaderFilename,
							char* FragmentShaderFilename);
};

