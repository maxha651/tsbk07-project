#pragma once
//
// Created by max on 2015-05-22.
//

#include <BaseComponent.h>
#include "Mirror.h"

class GameObject;

class MirrorContainer : public BaseComponent {

public:
    MirrorContainer();
    MirrorContainer(GameObject* gameObject, int width, int height, int res);
    MirrorContainer(GameObject* gameObject, const std::string& jsonPath);

    virtual void Awake() override;
    virtual void Start() override;
    virtual void Update() override;
    virtual void Render() override;

private:
    void Init();

    std::vector<Mirror> mirrors;
    std::vector<std::unique_ptr<GameObject>> mirrorObjects;
    float width, height, res;
    Eigen::Vector3f normal;
    Color color;

    // Keep last
    JSONLoader jsonLoader;
};


