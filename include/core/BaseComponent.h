#pragma once

//
// Created by max on 2015-04-20.
//

#include <Component.h>
#include <GOTransform.h>

/**
 * @brief Component almost superclass
 */
class BaseComponent : public Component {

public:
    BaseComponent();
    BaseComponent(const std::string& jsonPath);
    virtual ~BaseComponent();
    virtual void Update() override;
    virtual void Render() override;
    GOTransform& GetTransform();
private:
};


