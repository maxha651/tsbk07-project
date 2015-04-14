//
// Created by max on 2015-04-05.
//

#ifndef TSBK07_PROJECT_COMPONENT_H
#define TSBK07_PROJECT_COMPONENT_H

#include <GameObject.h>

/**
 * @brief Component superclass
 */
class Component {

public:
    virtual ~Component();
	GameObject GetGameObject();
	void SetGameObject(GameObject & go);
	void Update();
	GOTransform& GetTransform();
private:
	GameObject gameObject;
	
};



#endif //TSBK07_PROJECT_COMPONENT_H
