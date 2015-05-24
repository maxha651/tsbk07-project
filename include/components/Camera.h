#pragma once

//
// Created by Fredrik on 2015-04-07.
//

#include <string>

#include <Eigen/Geometry>

#include <BaseComponent.h>

/**
 * A camera class.
 */
class Camera : public BaseComponent
{
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    Camera();
    Camera(GameObject* gameObject, const std::string& jsonPath);
    ~Camera();

    virtual void Update() override;
    virtual void Render() override;
	virtual void Start() override;

    Eigen::Vector3f up;
    Eigen::Vector3f lookDir;

    Eigen::Matrix4f worldToViewMatrix;
    Eigen::Matrix4f projectionMatrix;

    Eigen::Vector3f cullFwd = 1000.0f * lookDir;
    Eigen::Vector3f cullWidth = 500.0f * lookDir.cross(up);

    static Eigen::Matrix4f LookAt(const Eigen::Vector3f& position,
                                  const Eigen::Vector3f& target, const Eigen::Vector3f& up);
    static Eigen::Matrix4f Perspective(float fovyInDegrees, float aspectRatio,
                                float znear, float zfar);
    static Eigen::Matrix4f Frustum(float left, float right, float bottom, float top,
                            float znear, float zfar);
private:
    void Init();
    void UpdateCullingBox();
    void UpdateInput();
    void UpdateUpVector();
    void UpdateWorldToView();

    // Keep all dependent variables before this
    JSONLoader jsonLoader;
};
