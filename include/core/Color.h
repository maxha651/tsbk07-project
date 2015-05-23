#pragma once 

#include <Eigen/Geometry>

using Eigen::Vector3f;
using Eigen::Vector4f;

//
// Created by Fredrik on 2015-05-23.
//

/**
 * @brief Class for a RGBA color.
 */
class Color {

public:
	Color();
	Color(float r, float g, float b, float a);
	virtual ~Color();
	Vector3f toVector3f();
	Vector4f toVector4f();
	float r, g, b, a;

private:
   
};

