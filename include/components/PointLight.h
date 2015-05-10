#pragma once
//
// Created by max on 2015-05-10.
//

#include <Eigen/Geometry>

#include <BaseComponent.h>

class PointLight : public BaseComponent {
public:
    PointLight();
    PointLight(const Eigen::Vector3f& position);
    virtual ~PointLight();

    void Update();
    void Render();

    Eigen::Vector3f position;
    Eigen::Vector4f color;
    float intensity, radius;
    size_t reference;

protected:

};


