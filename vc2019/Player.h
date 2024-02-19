#pragma once

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"


using namespace ci;
using namespace ci::app;

class Player 
{
public:
	Player();
	void setup();
	void update();
	void draw();


	gl::BatchRef mRect;
};

