#pragma once

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "Helpers.h"
#include "Hand.h"


using namespace ci;

class Player
{
public:
	Player();
	void setup();
	void update();
	void draw();
	void popN(int n);


	gl::BatchRef	mSphere;
	//gl::BatchRef mFingers[4][3];
	//gl::BatchRef mThumb[3];

private:
	Hand lHand = Hand(false);
	Hand rHand = Hand(true);
	vec3 handPos = vec3(0, 0, 0);
	vec3 fingerPos = vec3(0, 0, 0);
	//Finger names: Index: i, Middle: m, Ring: r, Pinky: p, Thumb: t
	vec4 fingerLen = vec4(0.9, 1.0, 0.95, 0.7);
	vec3 knuckleLen = vec3(0.4, 0.4, 0.2);
	vec3 thumbKnuckleLen = vec3(0.1, 0.4, 0.3);
	float thumbLen = 0.7;
};

