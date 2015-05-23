//
// Created by David on 2015-05-23.
//

#include <Radiosity.h>

Radiosity::Radiosity() {

}

Radiosity::~Radiosity() {

}

void Radiosity::CalculateRadiosity(){
	for (int i = 0; i < patches.size(); i++){
		Patch &currentPatch = patches[i];

		Color E = currentPatch.emittedEnergy;

		for (int j = 0; j < patches.size(); j++){
			if (!(i == j)){
				Patch targetedPatch = patches[j];
				RmReal from[3] = { (currentPatch.vert1.x() + currentPatch.vert2.x() + currentPatch.vert3.x()) / 3.0f,
					(currentPatch.vert1.y() + currentPatch.vert2.y() + currentPatch.vert3.y()) / 3.0f,
					(currentPatch.vert1.z() + currentPatch.vert2.z() + currentPatch.vert3.z()) / 3.0f };
				RmReal to[3] = { (targetedPatch.vert1.x() + targetedPatch.vert2.x() + targetedPatch.vert3.x()) / 3.0f,
					(targetedPatch.vert1.y() + targetedPatch.vert2.y() + targetedPatch.vert3.y()) / 3.0f,
					(targetedPatch.vert1.z() + targetedPatch.vert2.z() + targetedPatch.vert3.z()) / 3.0f };

				RmReal hitLocation[3] = { 0, 0, 0 };
				RmReal normal[3];
				RmReal hitDistance;
				int triangleIndex = -1;

				// Cast ray
				bool hit = rm->raycast(from, to, hitLocation, normal, &hitDistance, (RmUint32*)&triangleIndex);

				if (hit){
					// Calculate angles
					Vector3f rayCastLineBackward = Vector3f(from[0] - to[0], from[1] - to[1], from[2] - to[2]);
					float phi1 = rayCastLineBackward.dot(targetedPatch.normal) / rayCastLineBackward.norm()*targetedPatch.normal.norm();
				
					Vector3f rayCastLine = Vector3f(to[0] - from[0], to[1] - from[1], to[2] - from[2]);
					float phi2 = rayCastLine.dot(currentPatch.normal) / rayCastLine.norm()*currentPatch.normal.norm();

					Color B = targetedPatch.previousTotalEnergy;
					

				}

			}
		}
	}
}

void Radiosity::AddPatches(std::vector<Patch> p){
	patches.insert(patches.end(), p.begin(), p.end());
}

void Radiosity::CreateRayCastMesh(){

	for (int i = 0; i < patches.size(); i++){
		for (int j = 0; j < 3; j++){
			vertices.push_back(patches[i].vert1[j]);
		}
		for (int j = 0; j < 3; j++){
			vertices.push_back(patches[i].vert2[j]);
		}
		for (int j = 0; j < 3; j++){
			vertices.push_back(patches[i].vert3[j]);
		}
	}

	
	for (int i = 0; i < vertices.size(); i++){
		indices.push_back(i);
	}

	rm = createRaycastMesh(vertices.size(), &vertices[0], vertices.size() / 3, (const RmUint32 *)&indices[0]);
}