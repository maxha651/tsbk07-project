#pragma once

//
// Created by max on 2015-04-05.
//

class GOTransform;
class GameObject;

/**
 * @brief Component superclass
 */
class Component {

public:
    virtual ~Component();
    GameObject* GetGameObject();
    void SetGameObject(GameObject* go);
    virtual void Update();
protected:
    GameObject* gameObject;
};


