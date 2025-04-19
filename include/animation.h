#pragma once
#include <vector>
#include <glm/vec3.hpp>

#include "casteljau.h"
#include "../Assessment2/Assessment2/include/model.h"

struct Animation
{
	int model;
	float duration;
	float currentTime;
	float delay;
	float delayTimer;
	bool isPlaying;
	bool hasStarted;

	// Transformations
	std::vector<point> pathPoints;
	glm::vec3 startPosition;
	glm::vec3 endPosition;
	glm::vec3 startRotation;
	glm::vec3 endRotation;
};

std::vector<Animation> animations;

int createAnimation(int model, float duration, float delay, glm::vec3 endPos, glm::vec3 endRot)
{
	Animation anim;
	anim.model = model;
	anim.duration = duration;
	anim.currentTime = 0.f;
	anim.isPlaying = true;
	anim.delay = delay;
	anim.delayTimer = 0.f;
	anim.hasStarted = false;

	anim.startPosition = models.at(model).position;
	anim.startRotation = models.at(model).rotation;
	anim.endPosition = endPos;
	anim.endRotation = endRot;

	point p1, pMid, pMid2, p2;

	// Start point
	p1.x = anim.startPosition.x;
	p1.y = anim.startPosition.y;
	p1.z = anim.startPosition.z;

	// End point
	p2.x = anim.endPosition.x;
	p2.y = anim.endPosition.y;
	p2.z = anim.endPosition.z;

	// Middle control point (creates an arc)
	pMid.x = (p1.x + p2.x) / 2.0f;
	pMid.y = std::max(p1.y, p2.y);
	pMid.z = (p1.z + p2.z) / 2.0f;

	pMid2.x = p1.x;
	pMid2.y = anim.endPosition.y;
	pMid2.z = (p1.z + p2.z) / 2.0f;

	std::vector<point> ctrl_points;

	ctrl_points.push_back(p1);
	ctrl_points.push_back(pMid);
	ctrl_points.push_back(pMid2);
	ctrl_points.push_back(p2);

	std::vector<point> curve = EvaluateBezierCurve(ctrl_points, 64);
	anim.pathPoints = ctrl_points;

	animations.push_back(anim);
	return animations.size() - 1;
}

void updateAnimations(float deltaTime)
{
	for (auto& anim : animations) 
	{
		if (!anim.isPlaying) continue;

		// Handle delay before animation starts
		if (!anim.hasStarted) {
			anim.delayTimer += deltaTime;
			if (anim.delayTimer >= anim.delay) {
				anim.hasStarted = true;
			}
			else {
				continue; // Skip this frame if still in delay period
			}
		}

		// Update animation time
		anim.currentTime += deltaTime;

		// Handle animation completion
		if (anim.currentTime >= anim.duration) {
			setTranformations(anim.model, anim.endPosition, anim.endRotation, models.at(anim.model).scale);
			anim.isPlaying = false;
			continue;
		}

		// Calculate normalized time (0-1)
		float t = anim.currentTime / anim.duration;

		// Use casteljau to evaluate position on curve
		std::list<point> ctrlPoints(anim.pathPoints.begin(), anim.pathPoints.end());
		point currentPos = evaluate(t, ctrlPoints);

		// Linearly interpolate rotation and scale
		glm::vec3 currentPosition(currentPos.x, currentPos.y, currentPos.z);
		glm::vec3 currentRotation = anim.startRotation + t * (anim.endRotation - anim.startRotation);

		// Apply the transformation
		setTranformations(anim.model, currentPosition, currentRotation, models.at(anim.model).scale);
	}
}