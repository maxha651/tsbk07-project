#pragma once 

//
// Created by Fredrik on 2015-05-23.
//
#include <memory>
#include <vector>

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
	Vector3f totalEnergy;
	Vector3f totalEnergyTemp;
	Vector3f emittedEnergy;
	float reflectivity = 0;
	float area = 0;
	Vector3f color;
	std::vector<std::shared_ptr<Patch>> raycastingPatches;
	virtual ~Patch();

private:
	
};

