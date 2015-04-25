//
// Created by max on 2015-04-06.
//
// Fixed by Fredrik on 2015-04-08.
//

#include <GOTransform.h>

using Eigen::Vector3f;
using Eigen::Map;

const Vector3f GOTransform::right = Eigen::Vector3f::UnitX();
const Vector3f GOTransform::up = Eigen::Vector3f::UnitY();
const Vector3f GOTransform::forward = Eigen::Vector3f::UnitZ();

GOTransform::GOTransform() 
    : position{ 0.0f, 0.0f, 0.0f }, rotation{ 0.0f, 0.0f, 0.0f }, scale{ 1.0f, 1.0f, 1.0f } {
	
}

GOTransform::GOTransform(Vector3f pos, Vector3f rot, Vector3f scale) 
    : position(pos), rotation(rot), scale(scale) {

}

GOTransform::GOTransform(std::string jsonPath)
    : jsonLoader(jsonPath) {
    // XXX: This is kind of ugly but I'm confusing myself
    jsonLoader.AddArrayField("position", &jsonData[0]);
    jsonLoader.AddArrayField("rotation", &jsonData[1]);
    jsonLoader.AddArrayField("scale", &jsonData[2]);
    jsonLoader.Read();

    position    = {(float const &) jsonData[0][0],
                   (float const &) jsonData[0][1],
                   (float const &) jsonData[0][2]};
    position    = {(float const &) jsonData[1][0],
                   (float const &) jsonData[1][1],
                   (float const &) jsonData[1][2]};
    scale       = {(float const &) jsonData[2][0],
                   (float const &) jsonData[2][1],
                   (float const &) jsonData[2][2]};
};

GOTransform::~GOTransform() {

}

void GOTransform::SetPosition(float x, float y, float z) {
    this->position = { x, y, z };
}
void GOTransform::SetPosition(Vector3f position) {
    this->position = position;
}

Vector3f GOTransform::GetRotation() {
    // TODO: stub
    return Vector3f();
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


