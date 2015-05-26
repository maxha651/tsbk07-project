//
// Created by David and Fredrik on 2015-05-23.
//

#include <Radiosity.h>
#include <math.h>
#include <memory>

#include <GameObject.h>
#include <Context.h>
#include <Game.h>

Radiosity::Radiosity() {

}

Radiosity::~Radiosity() {

}

// Calculates energies and radiosity for all patches.
void Radiosity::CalculateRadiosity(){
	
	// LineRenderer for debugging
	GameObject &go = (Context::Instance().game)->GetGameObject("linerenderer");
	LineRenderer *l = go.GetComponent<LineRenderer>();

		// Grab all patches.
		std::cout << "starting raycasting" << std::endl;
		for (int i = 0; i < patches.size(); i++){
			Patch* currentPatch = patches[i].get();

			// For every patch we compare the patch with every other patch.
			for (int j = 0; j < patches.size(); j++){

				if (!(i == j)){
					Patch* targetedPatch = patches[j].get();

					// Calculate the middle position of the current patch.
					RmReal from[3] = { (currentPatch->vert1.x() + currentPatch->vert2.x() + currentPatch->vert3.x()) / 3.0f,
						(currentPatch->vert1.y() + currentPatch->vert2.y() + currentPatch->vert3.y()) / 3.0f,
						(currentPatch->vert1.z() + currentPatch->vert2.z() + currentPatch->vert3.z()) / 3.0f };

					// Calculate the middle position of the target patch.
					RmReal to[3] = { (targetedPatch->vert1.x() + targetedPatch->vert2.x() + targetedPatch->vert3.x()) / 3.0f,
						(targetedPatch->vert1.y() + targetedPatch->vert2.y() + targetedPatch->vert3.y()) / 3.0f,
						(targetedPatch->vert1.z() + targetedPatch->vert2.z() + targetedPatch->vert3.z()) / 3.0f };

					// Need to overshoot.
					/*to[0] = 2 * to[0] - from[0];
					to[1] = 2 * to[1] - from[1];
					to[2] = 2 * to[2] - from[2];*/
					to[0] = from[0] + 1.1f * (to[0] - from[0]);
					to[1] = from[1] + 1.1f * (to[1] - from[1]);
					to[2] = from[2] + 1.1f * (to[2] - from[2]);

				    // Move raycast slightly forward to not hit the patch itself
					from[0] = from[0] + 0.01f * (to[0] - from[0]);
					from[1] = from[1] + 0.01f * (to[1] - from[1]);
					from[2] = from[2] + 0.01f * (to[2] - from[2]);

					float v = currentPatch->normal.dot(targetedPatch->normal) / (currentPatch->normal.norm()*targetedPatch->normal.norm());

					// Do not share energy with patches that have the same "rotation" or "angle" as the current patch.
					if (v < 0.999){

						// Wether we should use raycast as visibility detection.
						if (usingRaycast){
							
							// Cast ray.
							RmReal hitLocation[3] = { 0, 0, 0 }; // We probably don't need to initialize this.
							RmReal normal[3]; // We do not use this. The library we use point the normal in wrong direction. Instead fetch normal from target patch.
							RmReal hitDistance; // The magnitude of the raycast from the from position to where we hit the target location.
							int triangleIndex = -1;
							bool hit = rm->raycast(from, to, hitLocation, normal, &hitDistance, (RmUint32*)&triangleIndex);
							if (hit && triangleIndex == j){ // triangleIndex == j to check if it was actually the target patch that we hit, and not some other patch.
								currentPatch->raycastingPatches.emplace_back(targetedPatch);
							}
						}
						else{
							currentPatch->raycastingPatches.emplace_back(targetedPatch);
						}
					
						// Debug.
						//l->AddLine(Vector3f(from[0], from[1], from[2]), Vector3f(to[0], to[1], to[2]));
						/*if (i == 25){
							Vector3f f = Vector3f(from[0], from[1], from[2]);
							Vector3f nn = f + currentPatch->normal;
							l->AddLine(f, nn);
							l->AddLine(Vector3f(from[0], from[1], from[2]), Vector3f(to[0], to[1], to[2]));
						}*/
					}
				}
			}
		}

		// Calculate the radiosity and energy bouncing.
		for (int k = 0; k < iterations; k++){
			std::cout << "Iteration: " << k << std::endl;

			// Add emitted energy to total energy.
			for (int i = 0; i < patches.size(); i++){
				Vector3f E = patches[i]->emittedEnergy;
				patches[i]->totalEnergy += E / (float)iterations;
			}
			
			// Iterate over all patches for every patch.
			for (int i = 0; i < patches.size(); i++){
				Patch* currentPatch = patches[i].get();

				// For every patch we compare the patch with every other patch.
				for (int j = 0; j < currentPatch->raycastingPatches.size(); j++){

					Patch* targetedPatch = currentPatch->raycastingPatches[j].get();

					// Calculate the middle position of the current patch.
					Vector3f from = Vector3f((currentPatch->vert1.x() + currentPatch->vert2.x() + currentPatch->vert3.x()) / 3.0f,
						(currentPatch->vert1.y() + currentPatch->vert2.y() + currentPatch->vert3.y()) / 3.0f,
						(currentPatch->vert1.z() + currentPatch->vert2.z() + currentPatch->vert3.z()) / 3.0f );

					// Calculate the middle position of the target patch.
					Vector3f to = Vector3f((targetedPatch->vert1.x() + targetedPatch->vert2.x() + targetedPatch->vert3.x()) / 3.0f,
						(targetedPatch->vert1.y() + targetedPatch->vert2.y() + targetedPatch->vert3.y()) / 3.0f,
						(targetedPatch->vert1.z() + targetedPatch->vert2.z() + targetedPatch->vert3.z()) / 3.0f); 

					// Calculate distance to hit from patch.
					float hitDistance = (from - to).norm();

					// Calculate angles from normal to patches.
					Vector3f rayCastLineBackward = Vector3f(from[0] - to[0], from[1] - to[1], from[2] - to[2]);
					float phi1 = std::fmax(0.0f, rayCastLineBackward.dot(targetedPatch->normal) / (rayCastLineBackward.norm()*targetedPatch->normal.norm()));

					Vector3f rayCastLine = Vector3f(to[0] - from[0], to[1] - from[1], to[2] - from[2]);
					float phi2 = std::fmax(0.0f, rayCastLine.dot(currentPatch->normal) / (rayCastLine.norm()*currentPatch->normal.norm()));

					// Calculate the total energy to a temp variable so that we can do multiple iterations with bouncing.
					// Using the radiosity equation described at wikipedia: http://en.wikipedia.org/wiki/Radiosity_(computer_graphics)
					float x = targetedPatch->reflectivity * currentPatch->color.x() * targetedPatch->totalEnergy.x() * (1.0f / (M_PI*hitDistance*hitDistance)) * phi1 * phi2 * targetedPatch->area / (float)iterations;
					float y = targetedPatch->reflectivity * currentPatch->color.y() * targetedPatch->totalEnergy.y() * (1.0f / (M_PI*hitDistance*hitDistance)) * phi1 * phi2 * targetedPatch->area / (float)iterations;
					float z = targetedPatch->reflectivity * currentPatch->color.z() * targetedPatch->totalEnergy.z() * (1.0f / (M_PI*hitDistance*hitDistance)) * phi1 * phi2 * targetedPatch->area / (float)iterations;
					Vector3f energy = Vector3f(x, y, z);
					currentPatch->totalEnergyTemp += energy;
				}

				// Finalize current patch total energy temp with the emitted energy and total energy from earlier iterations.
				currentPatch->totalEnergyTemp = currentPatch->totalEnergy + currentPatch->totalEnergyTemp;
			}

			// For each iteration we need to zero the temp energy and set the actual total energy to the total temp energy.
			for (int i = 0; i < patches.size(); i++){
				patches[i]->totalEnergy = patches[i]->totalEnergyTemp;
				patches[i]->totalEnergyTemp = Vector3f();
			}
		}
}


// Adds patches that we then can use when calculating radiosity.
void Radiosity::AddPatches(std::vector<std::shared_ptr<Patch>> p){
	patches.insert(patches.end(), p.begin(), p.end());
}

// Creates a raycast mesh from the patches that have been added. 
// The raycast mesh can then be used for the visibility detection when calculating radiosity energy.
void Radiosity::CreateRayCastMesh(){
	if (!usingRaycast) {
		return;
	}

	for (int i = 0; i < patches.size(); i++){
		for (int j = 0; j < 3; j++){
			vertices.push_back(patches[i]->vert1[j]);
		}
		for (int j = 0; j < 3; j++){
			vertices.push_back(patches[i]->vert2[j]);
		}
		for (int j = 0; j < 3; j++){
			vertices.push_back(patches[i]->vert3[j]);
		}
	}

	
	for (int i = 0; i < vertices.size(); i++){
		indices.push_back(i);
	}

	rm = createRaycastMesh(vertices.size(), &vertices[0], vertices.size() / 3, (const RmUint32 *)&indices[0]);
}