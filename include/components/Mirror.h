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
	virtual void Start() override;

private:
    void RenderTexture();
    void LoadVBOAndVAO(const Eigen::Vector3f& start, const Eigen::Vector3f& end, const Eigen::Vector3f& end2);

    unsigned int vertexArrayObjID;
    unsigned int rendererRef;
    unsigned int fboRef;
    unsigned int program;
    unsigned int texProgram;
    float width, height;
    std::vector<float> vertices, texCoords;
    std::vector<unsigned int> indices;
    Eigen::Vector3f normal;

    Camera camera;
    // Keep last
    JSONLoader jsonLoader;
};


