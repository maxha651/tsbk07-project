#pragma once 

//
// Created by Fredrik on 2015-05-23.
//

#include <Eigen/Geometry>
#include <Color.h>

using Eigen::Vector3f;

/**
 * @brief Class for a patch used with the radiosity algorithm.
 */
class Patch {

public:
	Patch(Vector3f v1, Vector3f v2, Vector3f v3, Vector3f normal);
	Vector3f vert1, vert2, vert3, normal;
	Color totalEnergy = Color(0,0,0,1);
	Color previousTotalEnergy = Color(0, 0, 0, 1); // The energy previous to each iteration of radiosity
	Color emittedEnergy = Color(0, 0, 0, 1);
	float reflectivity = 0;
	virtual ~Patch();

private:
	
};

