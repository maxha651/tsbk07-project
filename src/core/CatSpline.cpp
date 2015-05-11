#include <CatSpline.h>

//
// Created by Fredrik on 2015-05-01.
//

/**
* Class for an Overhauser/Catmull Rom Spline.
* Using a very similar implementation to the
* implementation described at codeproject.com:
* http://www.codeproject.com/Articles/30838/Overhauser-Catmull-Rom-Splines-for-Camera-Animatio
* and graphicsrunner.se:
* http://graphicsrunner.blogspot.se/2008/05/camera-animation-part-ii.html
*/

// Default constructor. Creates an empty spline. "You" need to add control / spline points "yourself".
// You also need to set delta_t correctly.
CatSpline::CatSpline() : spline_points(), delta_t(0)
{

}

// Copy constructor.
CatSpline::CatSpline(const CatSpline & s)
{
	for (int i = 0; i < s.spline_points.size(); i++) {
		spline_points.push_back(s.spline_points[i]);
	}
	delta_t = s.delta_t;
}

// Destructor.
CatSpline::~CatSpline()
{

}

// Computes a interpolated spline point.
// "Magic numbers" from matrix under "Interpolation on the unit interval without exact derivatives" in http://en.wikipedia.org/wiki/Cubic_Hermite_spline
//
// Try experimenting with these numbers. I'm not really sure what will happen. ./Fredrik
Vector3f CatSpline::Compute(float t, const Vector3f & p1, const Vector3f & p2, const Vector3f & p3, const Vector3f & p4)
{
	float t2 = t * t;
	float t3 = t * t * t;

	float b1 = .5 * (-t3 + 2 * t2 - t);
	float b2 = .5 * (3 * t3 - 5 * t2 + 2);
	float b3 = .5 * (-3 * t3 + 4 * t2 + t);
	float b4 = .5 * (t3 - t2);

	return (p1 * b1 + p2 * b2 + p3 * b3 + p4 * b4);
}

// Adds a spline point to the spline vector.
void CatSpline::AddSplinePoint(const Vector3f & v)
{
	// Add the spline point.
	spline_points.push_back(v);

	// Scale delta_t with size of splines.
	delta_t = (float) 1 / (float) spline_points.size();
}

Vector3f CatSpline::GetInterpolatedSplinePoint(float t)
{
	// Find out in which interval we should pick spline points from.
	int p = (int) (t / delta_t); // (t / delta_t) == (t * spline_points.size()) where t is in the interval [0, 1].

	// Used to compute local control point indices if we are outside the spline.
	#define BOUNDS(pp) { if (pp < 0) { pp = 0; } else if (pp >= (int)spline_points.size() - 1) pp = spline_points.size() - 1; }
	
	// Compute local control point indices.
	int p0 = p - 1;     
	BOUNDS(p0);
	
	int p1 = p;         
	BOUNDS(p1);
	
	int p2 = p + 1;     
	BOUNDS(p2);
	
	int p3 = p + 2;     
	BOUNDS(p3);
	
	float lt = (t - delta_t * (float) p) / delta_t;
	
	return CatSpline::Compute(lt, spline_points[p0], spline_points[p1], spline_points[p2], spline_points[p3]);
}

int CatSpline::GetNumberOfPoints()
{
	return spline_points.size();
}

Vector3f & CatSpline::GetNthPoint(int n)
{
	return spline_points[n];
}


