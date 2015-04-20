#pragma once

//
// Created by max on 2015-04-06.
//

#include <Eigen/Geometry>

using Eigen::Vector3f;

class GOTransform {

public:
    GOTransform();
    GOTransform(Vector3f pos, Vector3f rot, Vector3f scale);
    GOTransform(std::string path);
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

private:
    Vector3f position, rotation, scale;

};

