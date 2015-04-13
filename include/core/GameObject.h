//
// Created by max on 2015-04-05.
//

#ifndef TSBK07_PROJECT_GAMEOBJECT_H
#define TSBK07_PROJECT_GAMEOBJECT_H

#include <GameObject.h>

/**
 * @brief Class for any object in game, contains Components
 */
class GameObject {

public:
    GameObject();
    virtual ~GameObject();

    template<class T> T GetComponent();
    void AddComponent(Component);

private:
    std::vector<Component> components;

};


#endif //TSBK07_PROJECT_GAMEOBJECT_H
