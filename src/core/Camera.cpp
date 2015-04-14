//
// Created by Fredrik on 2015-04-07.
//

#include <Camera.h>
#include <GOTransform.h>
#include <Eigen/Geometry>

Camera::Camera()
{	
	this->up = GOTransform::up;
}


Camera::~Camera()
{
}

void Camera::Update(){

}
