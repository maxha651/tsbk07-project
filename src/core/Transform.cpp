//
// Created by max on 2015-04-06.
//

#include <Transform.h>

using Eigen::Vector3f;

const Vector3f Transform::right   = { 1.0f, 0.0f, 0.0f };
const Vector3f Transform::up      = { 0.0f, 1.0f, 0.0f };
const Vector3f Transform::forward = { 0.0f, 0.0f, 1.0f };

Transform::Transform() 
    : position{ 0.0f, 0.0f, 0.0f }, rotation{ 0.0f, 0.0f, 0.0f }, scale{ 1.0f, 1.0f, 1.0f } {

}

Transform::Transform(Vector3f pos, Vector3f rot, Vector3f scale) 
    : position(pos), rotation(rot), scale(scale) {

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


