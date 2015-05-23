//
// Created by Fredrik on 2015-05-23.
//

#include <Color.h>

Color::Color(float r, float g, float b, float a) {
	this->a = a;
	this->r = r;
	this->g = g;
	this->b = b;
}

Color::~Color() {
    
}

Vector3f Color::toVector3f() {
	return Vector3f(this->r, this->g, this->b);
}

Vector4f Color::toVector4f() {
	return Vector4f(this->r, this->g, this->b, this->a);
}