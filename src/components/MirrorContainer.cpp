//
// Created by max on 2015-05-22.
//

#include <MirrorContainer.h>

MirrorContainer::MirrorContainer() {

}

MirrorContainer::MirrorContainer(GameObject *gameObject, int width, int height, int res)
        : width(width), height(height), res(res) {
    this->gameObject = gameObject;
}

MirrorContainer::MirrorContainer(const std::string &jsonPath) {

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

}
