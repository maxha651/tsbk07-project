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
    Component();
    Component(GameObject *gameObject);
    virtual ~Component();
    virtual GameObject* GetGameObject();
    virtual void SetGameObject(GameObject* go);
    virtual void Update() = 0;
    virtual void Render() = 0;
    virtual void Awake() = 0;
	virtual void Start() = 0;
protected:
    GameObject* gameObject;
};


