#pragma once
//
// Created by max on 2015-05-10.
//


#include "PointLight.h"

class LitMaterial;

class RadiosityLight : public PointLight {

public:
    RadiosityLight();
    virtual ~RadiosityLight();

    void Update() override;
    void Render() override;

    void AddIncomingLight(PointLight* light);

protected:
    Eigen::Vector3f normal;

};
