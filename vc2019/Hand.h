#pragma once

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "Helpers.h"
#include <vector>


using namespace ci;

class Hand
{
public:
	Hand();
	Hand(bool rightHand, std::vector<float> angles);
	void setup();
	void update();
	void draw();
	void popN(int n);

	void setAngles(std::vector<float> angles);

	gl::BatchRef mRect;
	gl::BatchRef mFingers[4][3];
	gl::BatchRef mThumb[3];

private:
	bool rightHand;
	vec3 handPos;
	vec3 fingerPos;
	//Finger names: Index: i, Middle: m, Ring: r, Pinky: p, Thumb: t
	vec4 fingerLen = vec4(0.8, 1.0, 0.9, 0.65);
	vec3 knuckleLen = vec3(0.4, 0.4, 0.2);
	vec3 thumbKnuckleLen = vec3(0.1, 0.4, 0.3);
	float thumbLen = 0.7;
	// Defaulted to left hand
	vec3 thumbBaseTranslate = vec3(-0.4, 0, 0);
	float thumbBaseRotation = (M_PI / 2);
	vec3 thumbJointTranslate = vec3(-0.4, 0.2, 0);
	float thumbJointRotation = (M_PI / 2); //3
	vec3 thumbTipTranslate = vec3(0, 0.4, 0);
	float thumbTipRotation = (M_PI / 3); //6



	vec3 fingerTranslate[4];
	float fingerFlexion[4][3];
};

