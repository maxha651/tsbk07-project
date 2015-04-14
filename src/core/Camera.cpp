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
	Eigen::Matrix3f rotationMatrix;

	GOTransform t = this->GetTransform();
	Vector3f * rotation = & t.GetRotation();

	float rotationX = rotation->x;
	float rotationY = rotation->y;

	rotationMatrix = Eigen::AngleAxisf(rotation->x, Vector3f::UnitX()) * Eigen::AngleAxisf(rotation->y, Vector3f::UnitY()) * Eigen::AngleAxisf(rotation->z, Vector3f::UnitZ());

	this->up = rotationMatrix * GOTransform::up;
}

