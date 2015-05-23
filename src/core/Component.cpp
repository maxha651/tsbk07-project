//
// Created by max on 2015-04-05.
//

#include <Component.h>

Component::Component() : gameObject(nullptr) {

}

Component::Component(GameObject *gameObject) : gameObject(gameObject) {

}

Component::~Component() {

}

// Setters and getters
void Component::SetGameObject(GameObject* go){
	this->gameObject = go;
}

GameObject* Component::GetGameObject(){
	return this->gameObject;
}
