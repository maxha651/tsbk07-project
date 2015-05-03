//
// Created by Fredrik on 2015-04-07.
//

#include <Camera.h>

#include <Context.h>

using Eigen::Vector3f;
using Eigen::Matrix4f;
using Eigen::Translation3f;

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
    jsonLoader.AddArrayField<float>("lookDir", lookDir.data(), 3);
    jsonLoader.Read();
}

Camera::~Camera()
{
}

void Camera::Update() {
	UpdateUpVector();
    UpdateWorldToView();
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

void Camera::UpdateWorldToView() {
    Vector3f position = this->GetTransform().GetPosition();
    Vector3f target = position + lookDir;
    worldToViewMatrix = LookAt(position, target, up);
}

/**
 * \brief Based on Ingemar's PFNP book
 */
Matrix4f Camera::LookAt(const Vector3f &position, const Vector3f &target,
                               const Vector3f &up) {
    Vector3f dirVec = position - target;
    Vector3f rightVec = dirVec.cross(up);
    Vector3f orthoUp = rightVec.cross(dirVec);

    Matrix4f ret;
    ret <<  rightVec.transpose(), 0,
            orthoUp.transpose(), 0,
            dirVec.transpose(), 0,
            0, 0, 0, 0;

#if 0
    std::cout << "position: " << position << ", target: " << target << " up: " << up << std::endl;
    std::cout << "yields transform matrix: " << ret << std::endl;
#endif

    return ret;
}

void Camera::Render() {
    BaseComponent::Render();
}
