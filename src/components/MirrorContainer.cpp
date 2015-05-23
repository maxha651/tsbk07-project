//
// Created by max on 2015-05-22.
//

#include <MirrorContainer.h>

#include <Eigen/Geometry>

#include <GameObject.h>

using Eigen::Vector3f;
using Eigen::Vector4f;

MirrorContainer::MirrorContainer() {
    Init();
}

MirrorContainer::MirrorContainer(GameObject *gameObject, int width, int height, int res)
        : width(width), height(height), res(res) {
    Init();
}

MirrorContainer::MirrorContainer(const std::string &jsonPath) : jsonLoader(jsonPath) {
    jsonLoader.AddDataField("width", &width);
    jsonLoader.AddDataField("height", &height);
    jsonLoader.AddDataField("res", &res);
    jsonLoader.AddArrayField("normal", normal.data(), 3);
    jsonLoader.AddArrayField("color", color.data(), 4);
    jsonLoader.Read();

    Init();
}

void MirrorContainer::Start() {
    BaseComponent::Start();
    Vector3f start = GetTransform()->GetPosition();
    for (int y = 0; y < height; y += res) {
        for (int x = 0; x < width; x += res) {
            Vector3f localStart = start + GOTransform::right * x + GOTransform::up * y;
            //mirrors.emplace_back(res, res, normal, localStart, localStart + GOTransform::up * res,
            //                     localStart + GOTransform::right * res, color);
        }
    }

    for (auto& mirror : mirrors) {
        mirror.Start();
    }
}

void MirrorContainer::Update() {
    BaseComponent::Update();
    for (auto& mirror : mirrors) {
        mirror.Update();
    }
}

void MirrorContainer::Render() {
    BaseComponent::Render();
    for (auto& mirror : mirrors) {
        mirror.Render();
    }
}

void MirrorContainer::Init() {
}
