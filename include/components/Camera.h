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
    Camera();
    Camera(const std::string& jsonPath);
    ~Camera();

    virtual void Update() override;
    virtual void Render() override;

    Eigen::Vector3f up;
    Eigen::Vector3f lookDir;

    Eigen::Matrix4f worldToViewMatrix;
    Eigen::Matrix4f projectionMatrix;

private:
    void Init();
    void UpdateUpVector();
    void UpdateWorldToView();

    static Eigen::Matrix4f LookAt(const Eigen::Vector3f& position,
            const Eigen::Vector3f& target, const Eigen::Vector3f& up);
    Eigen::Matrix4f Perspective(float fovyInDegrees, float aspectRatio,
                                float znear, float zfar);
    Eigen::Matrix4f Frustum(float left, float right, float bottom, float top,
                            float znear, float zfar);

    JSONLoader jsonLoader;

    // We don't want any weird copy errors, should only ever be one Camera
    Camera(const Camera&) = delete;
    void operator=(const Camera&) = delete;
};
