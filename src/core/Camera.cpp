//
// Created by Fredrik on 2015-04-07.
//

#include <Camera.h>
#include <GOTransform.h>

Camera::Camera()
{	
	this->up = GOTransform::up;
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

Camera::Camera(const std::string &jsonPath) {

}
