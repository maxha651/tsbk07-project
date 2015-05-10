//
// Created by Fredrik on 2015-04-07.
//

#include <Camera.h>

#include <Context.h>
#include <Input.h>
#include <GameObject.h>

using Eigen::Vector3f;
using Eigen::Vector2f;
using Eigen::Matrix3f;
using Eigen::Matrix4f;
using Eigen::Translation3f;

// frustum parameters
static const float FRUSTUM_LEFT = -1.0f, FRUSTUM_RIGHT = 1.0f, FRUSTUM_BOTTOM = -1.0f,
				FRUSTUM_TOP = 1.0f, FRUSTUM_NEAR = 1.0f, FRUSTUM_FAR = 100.0f;
static const float MOUSE_MOVE_SCALER = 1.0f;

Camera::Camera()
{
    Init();
}

Camera::Camera(const std::string& jsonPath) :
	jsonLoader(jsonPath) {
    jsonLoader.AddArrayField<float>("lookDir", lookDir.data(), 3);
    jsonLoader.Read();
    Init();
}

void Camera::Init()
{
    if (Context::Instance().camera != nullptr) {
        std::cout << "Camera: Overriding existing camera" << std::endl;
    }
    Context::Instance().camera = this;
	this->up = GOTransform::up;
	this->projectionMatrix = Frustum(FRUSTUM_LEFT, FRUSTUM_RIGHT, FRUSTUM_BOTTOM, FRUSTUM_TOP, FRUSTUM_NEAR, FRUSTUM_FAR);
}

Camera::~Camera()
{
}

void Camera::Update() {
    UpdateInput();
    UpdateUpVector();
    UpdateWorldToView();
}

void Camera::UpdateInput() {
    if (!Input::IsInFocus()) {
        return;
    }

    Vector3f rightVec = lookDir.cross(up);
    Vector3f orthoUp = rightVec.cross(lookDir);
    rightVec.normalize();
    orthoUp.normalize();

    // Change look direction
    Vector2f mouseMove = Input::GetMouseMove();
    lookDir += rightVec * mouseMove.x();
    lookDir += orthoUp * mouseMove.y();
    // Change position
    Vector2f inputAxis = Input::GetInputAxis();
    Vector3f position = gameObject->transform.GetPosition();
    position += rightVec * inputAxis.x();
    position += lookDir.normalized() * inputAxis.y();
    gameObject->transform.SetPosition(position);
}

// Updates the up vector of the camera. This can then be used using glLookAt().
void Camera::UpdateUpVector(){
	Matrix3f rotationMatrix;

	GOTransform* t = this->GetTransform();
	Vector3f& rotation = t->GetRotation();

	rotationMatrix = Eigen::AngleAxisf(rotation.x(), Vector3f::UnitX()) * 
            Eigen::AngleAxisf(rotation.y(), Vector3f::UnitY()) * 
            Eigen::AngleAxisf(rotation.z(), Vector3f::UnitZ());

	this->up = rotationMatrix * GOTransform::up;
}

void Camera::UpdateWorldToView() {
	GameObject *go = this->gameObject;
	Vector3f& position =  this->GetTransform()->GetPosition();
    Vector3f target = position + lookDir;
    worldToViewMatrix = LookAt(position, target, up);
}

/**
 * \brief Creates a world-to-view matrix
 *
 * Based on Ingemar's PFNP book
 */
Matrix4f Camera::LookAt(const Vector3f &position, const Vector3f &target,
                               const Vector3f &up) {
    Vector3f dirVec = position - target;
    Vector3f rightVec = dirVec.cross(up);
    Vector3f orthoUp = rightVec.cross(dirVec);

    Matrix4f ret;
    // using this syntax is an easy way to set matrices.
    ret <<  rightVec.transpose(), 0,
            orthoUp.transpose(), 0,
            dirVec.transpose(), 0,
            0, 0, 0, 0;

    //std::cout << "position: " << position << ", target: " << target << " up: " << up << std::endl;
    //std::cout << "yields transform matrix: " << ret << std::endl;

    return ret;
}

/**
 * \brief Creates a projection matrix
 *
 * Taken from VectorUtils3 and converted to use Eigen.
 */
Eigen::Matrix4f Camera::Perspective(float fovyInDegrees, float aspectRatio,
                                    float znear, float zfar)
{
    float ymax, xmax;
    ymax = znear * tanf(fovyInDegrees * (float) M_PI / 360.0f);
    if (aspectRatio < 1.0)
    {
        ymax = znear * tanf(fovyInDegrees * (float) M_PI / 360.0f);
        xmax = ymax * aspectRatio;
    }
    else
    {
        xmax = znear * tanf(fovyInDegrees * (float) M_PI / 360.0f);
        ymax = xmax / aspectRatio;
    }

    return Frustum(-xmax, xmax, -ymax, ymax, znear, zfar);
}

/**
 * \brief Creates a projection matrix
 *
 * Taken from VectorUtils3 and converted to use Eigen.
 * Added some stuff for clarity.
 */
Eigen::Matrix4f Camera::Frustum(float left, float right, float bottom, float top,
                                float znear, float zfar)
{
    float temp, temp2, temp3, temp4;
    Eigen::Matrix4f matrix;

    temp = 2.0f * znear;
    temp2 = right - left;
    temp3 = top - bottom;
    temp4 = zfar - znear;
    // See Ingemar's PFNP p. 59
    float A = (right + left) / temp2;
    float B = (top + bottom) / temp3;
    float C = (-zfar - znear) / temp4;
    float D = (-temp * zfar) / temp4;

    matrix <<
            temp / temp2, 0.0f, 0.0f, 0.0f,
            0.0f, temp / temp3, 0.0f, 0.0f,
            A, B, C, -1.0f,
            0.0f, 0.0f, D, 0.0f;

    matrix.transposeInPlace();

    return matrix;
}

void Camera::Render() {
    BaseComponent::Render();
}
