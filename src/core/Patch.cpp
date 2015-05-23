//
// Created by Fredrik on 2015-05-23.
//

#include <Patch.h>

Patch::Patch(Vector3f v1, Vector3f v2, Vector3f v3, Vector3f n) {
	vert1 = v1;
	vert2 = v2;
	vert3 = v3;
	normal = n;
}

Patch::~Patch() {
    
}
