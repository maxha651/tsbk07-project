//
// Created by max on 2015-04-06.
//

#include <Transform.h>

Transform::Transform() 
    : position{ 0.0f, 0.0f, 0.0f }, rotation{ 0.0f, 0.0f, 0.0f }, scale{ 0.0f, 0.0f, 0.0f } {

}

Transform::Transform(vec3 pos, vec3 rot, vec3 scale)
        : position{pos}, rotation{rot}, scale{scale} {

}

Transform::~Transform() {

}
