//
// Created by max on 2015-05-22.
//

#include <MirrorContainer.h>

#include <Eigen/Geometry>

#include <GameObject.h>
#include <Game.h>

using Eigen::Vector3f;
using Eigen::Vector4f;

MirrorContainer::MirrorContainer() {
    Init();
}

MirrorContainer::MirrorContainer(GameObject *gameObject, int width, int height, int res)
        : width(width), height(height), res(res) {
    Init();
}

MirrorContainer::MirrorContainer(GameObject* gameObject, const std::string &jsonPath)
        : BaseComponent(gameObject), jsonLoader(jsonPath) {
    jsonLoader.AddDataField("width", &width);
    jsonLoader.AddDataField("height", &height);
    jsonLoader.AddDataField("res", &res);
    jsonLoader.AddArrayField("normal", normal.data(), 3);
    jsonLoader.AddArrayField("color", color.data(), 4);
    jsonLoader.Read();

    Init();
}

void MirrorContainer::Awake() {
    BaseComponent::Awake();
}

void MirrorContainer::Start() {
    BaseComponent::Start();
}

void MirrorContainer::Update() {
    BaseComponent::Update();
}

void MirrorContainer::Render() {
    BaseComponent::Render();
}

void MirrorContainer::Init() {
    Vector3f start = GetTransform()->GetPosition();
    for (int y = 0; y < height; y += res) {
        for (int x = 0; x < width; x += res) {
            Vector3f localStart = start + GOTransform::right * x + GOTransform::up * y;
            //mirrors.emplace_back(new Mirror(res, res, normal, localStart,
            //                                localStart + GOTransform::up * res,
            //                                localStart + GOTransform::right * res, color));
            //GameObject* newGameObject = new GameObject();
            //newGameObject->transform.SetPosition(localStart);
            //Context::Instance().game->AddGameObject(newGameObject);
        }
    }
}
