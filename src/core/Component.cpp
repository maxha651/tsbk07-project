//
// Created by max on 2015-04-05.
//

#include <Component.h>


Component::~Component() {

}

void Component::SetGameObject(GameObject go){
	this->gameObject = go;
}

