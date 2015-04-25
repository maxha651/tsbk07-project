#pragma once

//
// Created by max on 2015-04-20.
//

#include <GOTransform.h>
#include <Component.h>
#include <GameObject.h>

/**
 * @brief Component almost superclass
 */
class BaseComponent : public Component {

public:
    BaseComponent();
    BaseComponent(const std::string& jsonPath);
    virtual ~BaseComponent();
    virtual void Update() override;
    GOTransform& GetTransform();
private:
};


