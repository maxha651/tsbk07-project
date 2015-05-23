#pragma once 

//
// Created by David on 2015-05-23.
//

// C++ std lib
#include <cstdlib>
#include <vector>
#include <cstdio>

// Third party
#include <Eigen/Geometry>

// Ours
#include <Color.h>
#include <Patch.h>
#include <RaycastMesh.h>

using Eigen::Vector3f;

/**
* @Class to calculate radiosity for given modells/patches
*/
class Radiosity {

public:
	
	Radiosity();
	virtual ~Radiosity();
	void AddPatches(std::vector<Patch> p);
	void CalculateRadiosity();
	void CreateRayCastMesh();

	RaycastMesh *rm;
	std::vector<Patch> patches;
	std::vector<int> indices;
	std::vector<float> vertices;
	int iterations = 1;

private:

};

