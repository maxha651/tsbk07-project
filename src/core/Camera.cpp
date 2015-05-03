//
// Created by Fredrik on 2015-04-07.
//

#include <Camera.h>

#include <GOTransform.h>
#include <Context.h>


Camera::Camera()
{
    if (Context::Instance().camera != nullptr) {
        std::cout << "Camera: Overriding existing camera" << std::endl;
    }
    Context::Instance().camera = this;
	this->up = GOTransform::up;
}

Camera::Camera(const std::string& jsonPath) :
	jsonLoader(jsonPath) {
	// Initialize JSON data
}

Camera::~Camera()
{
}

void Camera::Update(){
	UpdateUpVector();
}

// Updates the up vector of the camera. This can then be used using glLookAt().
void Camera::UpdateUpVector(){
	Eigen::Matrix3f rotationMatrix;

	GOTransform t = this->GetTransform();
	Vector3f rotation = t.GetRotation();

	rotationMatrix = Eigen::AngleAxisf(rotation.x(), Vector3f::UnitX()) * 
            Eigen::AngleAxisf(rotation.y(), Vector3f::UnitY()) * 
            Eigen::AngleAxisf(rotation.z(), Vector3f::UnitZ());

	this->up = rotationMatrix * GOTransform::up;
}
