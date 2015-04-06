//
// Created by max on 2015-04-06.
//

#ifndef TSBK07_PROJECT_TRANSFORM_H
#define TSBK07_PROJECT_TRANSFORM_H

#include <VectorUtils3.h>

class Transform {

public:
    Transform();
    Transform(vec3 pos, vec3 rot, vec3 scale);
    virtual ~Transform();

    vec3 position;
    vec3 rotation;
    vec3 scale;

};


#endif //TSBK07_PROJECT_TRANSFORM_H
