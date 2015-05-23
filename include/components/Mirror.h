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
    Mirror(float width, float height, const Eigen::Vector3f& normal,
           const Eigen::Vector3f& start, const Eigen::Vector3f& left, Eigen::Vector3f& right);
    Mirror(const std::string& jsonPath);
    void Init();

    virtual void Update() override;
    virtual void Render() override;
	virtual void Start() override;

private:
    void RenderTexture();
    void LoadVBOAndVAO();

    unsigned int vertexArrayObjID;
    unsigned int rendererRef;
    unsigned int fboRef;
    unsigned int program;
    unsigned int texProgram;
    float width, height;
    std::vector<float> vertices, texCoords;
    std::vector<unsigned int> indices;
    Eigen::Vector3f start, right, left;
    Eigen::Vector3f normal;

    Camera camera;
    // Keep last
    JSONLoader jsonLoader;
};


