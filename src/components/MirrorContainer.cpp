//
// Created by max on 2015-05-22.
//

#include <MirrorContainer.h>

#include <Eigen/Geometry>

using Eigen::Vector3f;
using Eigen::Vector4f;

MirrorContainer::MirrorContainer() {
    Init();
}

MirrorContainer::MirrorContainer(GameObject *gameObject, int width, int height, int res)
        : width(width), height(height), res(res) {
    this->gameObject = gameObject;
    Init();
}

MirrorContainer::MirrorContainer(const std::string &jsonPath) : jsonLoader(jsonPath) {
    jsonLoader.AddDataField("width", &width);
    jsonLoader.AddDataField("height", &height);
    jsonLoader.AddDataField("res", &res);
    jsonLoader.Read();

    Init();
}

void MirrorContainer::Start() {
    BaseComponent::Start();
    Vector3f start = GetTransform()->GetPosition();
    for (int y = 0; y < height; y += res) {
        for (int x = 0; x < width; x += res) {
            Vector3f localStart = start + GOTransform::right * x + GOTransform::up * y;
            mirrors.emplace_back(res, res, normal, localStart, localStart + GOTransform::up * res,
                                 localStart + GOTransform::right * res, color);
        }
    }
}

void MirrorContainer::Update() {
    BaseComponent::Update();
}

void MirrorContainer::Render() {
    BaseComponent::Render();
}

void MirrorContainer::Init() {
}
