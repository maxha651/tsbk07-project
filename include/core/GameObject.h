//
// Created by max on 2015-04-05.
//

#ifndef TSBK07_PROJECT_GAMEOBJECT_H
#define TSBK07_PROJECT_GAMEOBJECT_H

#include <vector>
#include <Transform.h>

class Component;

/**
 * @brief Class for any object in game, contains Components
 */
class GameObject {

public:
	GameObject();
    virtual ~GameObject();

    template<class T> T GetComponent();
    void AddComponent(Component);
	void Update();
	Transform transform; 
private:
    std::vector<Component> components;

};


#endif //TSBK07_PROJECT_GAMEOBJECT_H
