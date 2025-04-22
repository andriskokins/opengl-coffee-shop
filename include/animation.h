#pragma once
#include <vector>
#include <glm/vec3.hpp>

#include "casteljau.h"
#include "light.h"
#include "model.h"

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
bool activeAnimation = false;

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

	// Use control points to make a smoother curve
	std::vector<point> curve = EvaluateBezierCurve(ctrl_points, 64);
	anim.pathPoints = ctrl_points;

	animations.push_back(anim);

	printf("Animation: Added animation for model %i\n", anim.model);
	printf("Animation: Start pos: x:%.2f y:%2.f z:%2.f\n", anim.startPosition.x, anim.startPosition.y, anim.startPosition.z);
	printf("Animation: End pos: x:%2.f y:%2.f z:%2.f\n", anim.endPosition.x, anim.endPosition.y, anim.endPosition.z);
	printf("Animation: Start time:%2.f Duration time:%2.f\n", anim.delay, anim.duration);

	return animations.size() - 1;
}

void updateAnimations(float deltaTime)
{
	// Add this static variable to track accumulated time between shadow updates
	static float shadowUpdateTimer = 0.0f;
	shadowUpdateTimer += deltaTime;

	// Update shadow maps if animation is active
	if (shadowUpdateTimer >= 0.025f && activeAnimation) 
	{
		for (auto& light : lights)
			light.shadow.updateShadow = true;
		shadowUpdateTimer = 0.0f; // Reset timer after update
	}

	for (auto& anim : animations) 
	{
		if (!anim.isPlaying) continue;

		// Handle delay before animation starts
		if (!anim.hasStarted) {
			anim.delayTimer += deltaTime;
			if (anim.delayTimer >= anim.delay) {
				activeAnimation = true;
				anim.hasStarted = true;
				printf("Animation: Started animation for model %i\n", anim.model);
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
			activeAnimation = false;
			continue;
		}

		// Calculate normalized time (0-1)
		float t = anim.currentTime / anim.duration;

		// Use casteljau to evaluate position on curve
		std::list<point> points(anim.pathPoints.begin(), anim.pathPoints.end());
		point currentPos = evaluate(t, points);

		// Linearly interpolate rotation and scale
		glm::vec3 currentPosition(currentPos.x, currentPos.y, currentPos.z);
		glm::vec3 currentRotation = anim.startRotation + t * (anim.endRotation - anim.startRotation);

		// Apply the transformation
		setTranformations(anim.model, currentPosition, currentRotation, models.at(anim.model).scale);
	}
}