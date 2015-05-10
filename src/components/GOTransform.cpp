//
// Created by max on 2015-04-06.
//
// Fixed by Fredrik on 2015-04-08.
//

#include <GOTransform.h>

using Eigen::Vector3f;
using Eigen::Matrix4f;
using Eigen::Map;
using Eigen::AngleAxisf;

const Vector3f GOTransform::right = Eigen::Vector3f::UnitX();
const Vector3f GOTransform::up = Eigen::Vector3f::UnitY();
const Vector3f GOTransform::forward = Eigen::Vector3f::UnitZ();


GOTransform::GOTransform() 
    : position{ 0.0f, 0.0f, 0.0f }, rotation{ 0.0f, 0.0f, 0.0f }, scale{ 1.0f, 1.0f, 1.0f } {
	
}

GOTransform::GOTransform(const GOTransform &transform)
    : position(transform.position), rotation(transform.rotation), scale(transform.scale) {

}

GOTransform::GOTransform(Vector3f pos, Vector3f rot, Vector3f scale) 
    : position(pos), rotation(rot), scale(scale) {

}

GOTransform::GOTransform(const std::string& jsonPath)
    : jsonLoader(jsonPath) {
    // XXX: This is kind of ugly but I'm confusing myself
    jsonLoader.AddArrayField("position", position.data(), 3);
    jsonLoader.AddArrayField("rotation", rotation.data(), 3);
    jsonLoader.AddArrayField("scale", scale.data(), 3);
    jsonLoader.Read();
};

GOTransform::~GOTransform() {

}

void GOTransform::SetPosition(float x, float y, float z) {
    this->position = { x, y, z };
}
void GOTransform::SetPosition(Vector3f position) {
    this->position = position;
}

Vector3f& GOTransform::GetPosition() {
    return position;
}

Vector3f& GOTransform::GetRotation() {
    return rotation;
}

Vector3f& GOTransform::GetScale() {
    return scale;
}

void GOTransform::SetRotation(float x, float y, float z) {
    this->rotation = { x, y, z };
}
void GOTransform::SetRotation(Vector3f rotation) {
    this->rotation = rotation;
}

void GOTransform::SetScale(float x, float y, float z) {
    this->scale = { x, y, z };
}
void GOTransform::SetScale(Vector3f scale) {
    this->scale = scale;
}

Matrix4f GOTransform::GetMatrix() {
    Eigen::Affine3f res;
    res.setIdentity();
    res.rotate(AngleAxisf(rotation.x(), right)
               * AngleAxisf(rotation.y(), up)
               * AngleAxisf(rotation.z(), forward));
    res.translate(position);
    res.scale(scale);

    return res.matrix();
}

