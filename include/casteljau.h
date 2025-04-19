#include <list>
#include <vector>
#include <algorithm>

#include "point.h"

point evaluate(float t, std::list<point> P)
{
	std::list<point> Q = P;

	while (Q.size() > 1)
	{
		std::list<point>::iterator p1 = Q.begin();
		std::list<point>::iterator p2 = std::next(p1);
		std::list<point> R;

		while (p2 != Q.end())
		{
			point p = operator+(operator*(1 - t, *p1), (operator*(t, *p2)));
			R.push_back(p);
			++p1;
			++p2;
		}
		Q.clear();
		Q = R;
	}

	return Q.front();
}

std::vector<point> EvaluateBezierCurve(std::vector<point>ctrl_points, int num_evaluations)
{
	std::list<point> ps(ctrl_points.begin(), ctrl_points.end());
	std::vector<point> curve;

	float offset = 1.f / num_evaluations;
	curve.push_back(ps.front());

	for (int e = 0; e <= num_evaluations - 1; e++)
	{
		point p = evaluate(offset * (e + 1), ps);
		curve.push_back(p);
	}

	return curve;
}

float* MakeFloatsFromVector(std::vector<point> curve, int &num_verts, int &num_floats, float r, float g, float b)
{
	num_verts = curve.size();
	if (num_verts == 0)
	{
		return NULL;
	}

	num_floats = num_verts * 6;

	float* vertexAttributes = static_cast<float*>(malloc(num_floats * sizeof(float))) ;

	int index = 0;
	for (auto p : curve)
	{
		// position
		vertexAttributes[index + 0] = p.x;
		vertexAttributes[index + 1] = p.y;
		vertexAttributes[index + 2] = p.z;
		// colour
		vertexAttributes[index + 3] = r;
		vertexAttributes[index + 4] = g;
		vertexAttributes[index + 5] = b;

		index += 6;
	}

	return vertexAttributes;
}

