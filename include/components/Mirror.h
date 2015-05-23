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
#include <Color.h>

class GameObject;

class Mirror : public BaseComponent {

public:
    Mirror();
    Mirror(float width, float height, const Eigen::Vector3f& normal, const Eigen::Vector3f& start,
           const Eigen::Vector3f& left, const Eigen::Vector3f& right, const Color& color);
    Mirror(GameObject* gameObject, const std::string& jsonPath);
    void Init();

    virtual void Update() override;
    virtual void Render() override;
    virtual void Awake() override;
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
    Color color;

    Camera camera;
    // Keep last
    JSONLoader jsonLoader;
};


