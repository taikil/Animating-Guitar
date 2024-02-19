#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Shader.h"
#include "cinder/Easing.h"

#include "MusicSimulator.h"
#include "Player.h"

using namespace ci;
using namespace ci::app;

class GuitarApp : public cinder::app::App {
public:
	void setup() override;
	void mouseDown(MouseEvent event) override;
	void update() override;
	void draw() override;

	static const int NUM_SLICES = 12;

	CameraPersp		mCam;
	gl::BatchRef	mSlices[NUM_SLICES];

private:
	MusicSimulator mMusicSim;
	Player player = Player();

};

void GuitarApp::setup()
{
	auto lambert = gl::ShaderDef().lambert().color();
	gl::GlslProgRef	shader = gl::getStockShader(lambert);
	player.setup();

	for (int i = 0; i < NUM_SLICES; ++i) {
		float rel = i / (float)NUM_SLICES;
		float sliceHeight = 1.0f / NUM_SLICES;
		auto slice = geom::Cube().size(1, sliceHeight, 1);
		auto trans = geom::Translate(0, rel, 0);
		auto color = geom::Constant(geom::COLOR,
			Color(CM_HSV, rel, 1, 1));
		mSlices[i] = gl::Batch::create(slice >> trans >> color,
			shader);
	}

	mCam.lookAt(vec3(2, 3, -10), vec3(0, 0.5f, 0));
	//mCam.lookAt(vec3(2, 3, -4), vec3(4, 2, 4));
}

void GuitarApp::mouseDown(MouseEvent event)
{
}

void GuitarApp::update()
{
}

void GuitarApp::draw()
{
	gl::clear();
	gl::enableDepthRead();
	gl::enableDepthWrite();

	gl::setMatrices(mCam);

	const float delay = 0.25f;
	// time in seconds for one slice to rotate
	const float rotationTime = 1.5f;
	// time in seconds to delay each slice's rotation
	const float rotationOffset = 0.1f; // seconds
	// total time for entire animation
	const float totalTime = delay + rotationTime +
		NUM_SLICES * rotationOffset;

	// loop every 'totalTime' seconds
	float time = fmod(getElapsedFrames() / 30.0f, totalTime);

	player.draw();

	for (int i = 0; i < NUM_SLICES; ++i) {
		// animates from 0->1
		float rotation = 0;
		// when does the slice begin rotating
		float startTime = i * rotationOffset;
		// when does it complete
		float endTime = startTime + rotationTime;
		// are we in the middle of our time section?
		if (time > startTime && time < endTime)
			rotation = (time - startTime) / rotationTime;
		// ease fn on rotation, then convert to radians
		float angle = easeInOutQuint(rotation) * M_PI / 2.0f;

		gl::ScopedModelMatrix scpModelMtx;
		gl::rotate(angleAxis(angle, vec3(0, 1, 0)));
		mSlices[i]->draw();
	}
}
CINDER_APP(GuitarApp, RendererGl)
