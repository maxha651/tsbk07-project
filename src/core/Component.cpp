//
// Created by max on 2015-04-05.
//

#include <Component.h>


Component::~Component() {

}

GOTransform & Component::GetTransform(){
	return this->gameObject.transform;
}

void Component::Update(){

}

// Setters and getters
void Component::SetGameObject(GameObject & go){
	this->gameObject = go;
}

GameObject & Component::GetGameObject(){
	return this->gameObject;
}
