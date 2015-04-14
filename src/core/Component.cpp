//
// Created by max on 2015-04-05.
//

#include <Component.h>


Component::~Component() {

}

GOTransform & Component::GetTransform(){
	return this->gameObject.transform;
}

void Component::SetGameObject(GameObject & go){
	this->gameObject = go;
}

