#pragma once

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "Trapezoid.h"


using namespace ci;

class Player
{
public:
	Player();
	void setup();
	void update();
	void draw();
	void popN(int n);


	gl::BatchRef mRect;
	gl::BatchRef mFingers[4][3];

private:
	vec3 handPos;
	vec3 fingerPos;
	//Finger names: Index: i, Middle: m, Ring: r, Pinky: p, Thumb: t
	vec4 fingerLen = vec4(0.9, 1.0, 0.95, 0.7);
	vec3 knuckleLen = vec3(0.4, 0.4, 0.2);
};

