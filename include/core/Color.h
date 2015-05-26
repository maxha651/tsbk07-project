#pragma once 

#include <Eigen/Geometry>

//
// Created by Fredrik on 2015-05-23.
//

/**
 * @brief Class for a RGBA color.
 */
class Color : public Eigen::Vector4f {

public:
	Color() : Eigen::Vector4f() {}
	Color(const Eigen::Vector4f& vec) : Eigen::Vector4f(vec) {}
	Color(float r, float g, float b, float a) : Eigen::Vector4f(r, g, b, a) {}
	float r() { return x(); }
	float g() { return y(); }
	float b() { return z(); }
	float a() { return w(); }
};

