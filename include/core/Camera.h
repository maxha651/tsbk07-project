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
    Eigen::Vector3f up;
    virtual void Update() override;

private:
    void UpdateUpVector();
    JSONLoader jsonLoader;

    // We don't want any weird copy errors
    Camera(const Camera&) = delete;
    void operator=(const Camera&) = delete;
};
