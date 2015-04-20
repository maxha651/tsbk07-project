//
// Created by max on 2015-04-20.
//

#include <BaseComponent.h>

BaseComponent::~BaseComponent() {

}

void BaseComponent::Update(){

}

GOTransform& BaseComponent::GetTransform() {
    return this->gameObject->transform;
}

