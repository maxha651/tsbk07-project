//
// Created by max on 2015-04-20.
//

#include <BaseComponent.h>

#include <cassert>

BaseComponent::BaseComponent(const std::string &jsonPath) {
    std::cerr << "Component doesn't implement JSON constructor :(" << std::endl;
    assert(false);
}

BaseComponent::BaseComponent() {

}

BaseComponent::~BaseComponent() {

}

void BaseComponent::Update(){

}

GOTransform& BaseComponent::GetTransform() {
    return this->gameObject->transform;
}

