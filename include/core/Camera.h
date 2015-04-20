#pragma once

//
// Created by Fredrik on 2015-04-07.
//

#include <Eigen/Geometry>

#include <BaseComponent.h>

/**
 * A camera class.
 */
class Camera : public BaseComponent
{
    public:
        Camera();
        ~Camera();
        Eigen::Vector3f up; 
        virtual void Update() override;

    private:
        void UpdateUpVector();
};
