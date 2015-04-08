//
// Created by max on 2015-04-06.
//
// Fixed by Fredrik on 2015-04-08.
//

#include <Transform.h>

using Eigen::Vector3f;
using Eigen::Map;

const Vector3f Transform::right = Eigen::Vector3f::UnitX();
const Vector3f Transform::up = Eigen::Vector3f::UnitY();
const Vector3f Transform::forward = Eigen::Vector3f::UnitZ();

Transform::Transform() 
    : position{ 0.0f, 0.0f, 0.0f }, rotation{ 0.0f, 0.0f, 0.0f }, scale{ 1.0f, 1.0f, 1.0f } {
	
}

Transform::Transform(Vector3f pos, Vector3f rot, Vector3f scale) 
    : position(pos), rotation(rot), scale(scale) {

}

Transform::Transform(std::string path) {
	// TODO
}

Transform::~Transform() {

}

void Transform::SetPosition(float x, float y, float z) {
    this->position = { x, y, z };
}
void Transform::SetPosition(Vector3f position) {
    this->position = position;
}

void Transform::SetRotation(float x, float y, float z) {
    this->rotation = { x, y, z };
}
void Transform::SetRotation(Vector3f rotation) {
    this->rotation = rotation;
}

void Transform::SetScale(float x, float y, float z) {
    this->scale = { x, y, z };
}
void Transform::SetScale(Vector3f scale) {
    this->scale = scale;
}


