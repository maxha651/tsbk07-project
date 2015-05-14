#pragma once
//
// Created by max on 2015-05-14.
//

#include <vector>
#include <string>

#include <Eigen/Geometry>

#include <BaseComponent.h>
#include <JSONLoader.h>
#include <Camera.h>

class Mirror : public BaseComponent {

public:
    Mirror();
    Mirror(const std::string& jsonPath);
    void Init();

    virtual void Update() override;
    virtual void Render() override;

private:
    void RenderTexture();
    void LoadVBOAndVAO();

    unsigned int rendererRef;
    unsigned int fboRef;
    float width, height;
    Eigen::Vector3f normal;

    Camera camera;
    // Keep last
    JSONLoader jsonLoader;
};


