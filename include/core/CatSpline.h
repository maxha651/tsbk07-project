#include <Eigen/Geometry>
#include <vector>

//
// Created by Fredrik on 2015-05-01.
//

using Eigen::Vector3f;

/**
* Class for an Overhauser/Catmull Rom Spline.
* Using a very similar implementation to the 
* implementation described at codeproject.com:
* http://www.codeproject.com/Articles/30838/Overhauser-Catmull-Rom-Splines-for-Camera-Animatio
* and graphicsrunner.se: 
* http://graphicsrunner.blogspot.se/2008/05/camera-animation-part-ii.html
*/
class CatSpline{
public: 
	CatSpline();
	CatSpline(const CatSpline & spline);
	~CatSpline();

	void AddSplinePoint(const Vector3f & v);
	Vector3f GetInerpolatedSplinePoint(float t);
	int GetNumberOfPoints();
	Vector3f & GetNthPoint(int n);

private: 
	std::vector<Vector3f> vp;
	float delta_t;
};