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

	// Constructors / destructors.
	CatSpline();
	CatSpline(const CatSpline & spline);
	~CatSpline();

	// Adds a spline point to the spline vector.
	void AddSplinePoint(const Vector3f & v);

	// Returns an interpolated spline point using the spline vector.
	Vector3f GetInterpolatedSplinePoint(float t);

	// Returns the number of points in the spline vector.
	int GetNumberOfPoints();

	// Returns the Nth point from the spline vector.
	Vector3f & GetNthPoint(int n);

	// Computes the Catmull-Rom equation at a given time t for a point quadruple (p1, p2, p3, p4).
	static Vector3f Compute(float t, const Vector3f & p1, const Vector3f & p2, const Vector3f & p3, const Vector3f & p4);

private: 
	// Contains all spline points.
	std::vector<Vector3f> spline_points;

	// Used to fetch 4 correct spline points when computing the Catmull-Rom equation.
	float delta_t;
};