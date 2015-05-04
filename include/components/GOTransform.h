#pragma once

//
// Created by max on 2015-04-06.
//

#include <Eigen/Geometry>

#include "JSONLoader.h"
#include "BaseComponent.h"

using Eigen::Vector3f;

class GOTransform : public Component {

public:
    GOTransform();
    GOTransform(const GOTransform& transform);
    GOTransform(Vector3f pos, Vector3f rot, Vector3f scale);
    GOTransform(const std::string& jsonPath);
    virtual ~GOTransform();

    Vector3f GetPosition();
    Vector3f GetRotation();
    Vector3f GetScale();

    void SetPosition(float x, float y, float z);
    void SetPosition(Vector3f);
    void SetRotation(float x, float y, float z);
    void SetRotation(Vector3f);
    void SetScale(float x, float y, float z);
    void SetScale(Vector3f);

    static const Vector3f right;
    static const Vector3f up;
    static const Vector3f forward;

    virtual void Update() override {}
    virtual void Render() override {}

private:
    Vector3f position, rotation, scale;
    // Keep all dependent variables before this
    JSONLoader jsonLoader;
};

