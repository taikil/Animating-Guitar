#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Shader.h"
#include "cinder/Easing.h"
#include "cinder/Camera.h"

#include "MusicSimulator.h"
#include "Player.h"

using namespace ci;
using namespace ci::app;

class GuitarApp : public cinder::app::App {
public:
	void setup() override;
	void mouseDown(MouseEvent event) override;
	void mouseDrag(MouseEvent event) override;
	void update() override;
	void draw() override;


	static const int NUM_SLICES = 12;

	CameraPersp		mCam;
	gl::BatchRef	mSlices[NUM_SLICES];

private:
	MusicSimulator mMusicSim;
	Player player = Player();
	vec2 mInitialMousePos;
	quat mRotationQuat;
	vec3 eyePos = vec3(0, 0, -5);
};

void GuitarApp::setup()
{
	setWindowSize(1920, 1080);
	mCam.lookAt(eyePos, vec3(0));
	mCam.setPerspective(60.0f, getWindowAspectRatio(), 0.1f, 1000.0f);
	mRotationQuat = quat();

	auto lambert = gl::ShaderDef().lambert().color();
	gl::GlslProgRef	shader = gl::getStockShader(lambert);

	player.setup();

}

void GuitarApp::mouseDown(MouseEvent event)
{
	mInitialMousePos = event.getPos();
}

void GuitarApp::mouseDrag(MouseEvent event) {
	// Calculate the mouse movement delta
	if (event.isRightDown()) {
		vec2 mouseDelta = vec2(event.getPos()) - mInitialMousePos;

		// Create a quaternion representing the rotation based on mouse movement
		quat rotationQuat = quat(vec3(-mouseDelta.y, -mouseDelta.x, 0) * 0.01f);

		// Apply the rotation to the camera's orientation
		mRotationQuat = rotationQuat * mRotationQuat;

		// Adjust the camera's position to rotate around the point of interest
		vec3 eye = mRotationQuat * eyePos; // Eye position relative to target
		vec3 target = vec3(0); // Your point of interest
		mCam.lookAt(eye + target, target); // Update camera's lookAt and eye positions
		mInitialMousePos = event.getPos();
	}
	else {
		vec2 mouseDelta = vec2(event.getPos()) - mInitialMousePos;

		// Adjust the camera's position based on the mouse movement for zooming
		float zoomSpeed = 0.1f;
		eyePos.z += mouseDelta.y * zoomSpeed;

		// Ensure that the camera does not go too close or too far
		eyePos.z = math<float>::clamp(eyePos.z, -20.0f, -1.0f);

		// Update the camera's lookAt and eye positions
		mCam.lookAt(eyePos, vec3(0));
	}
}

void GuitarApp::update()
{
}

void GuitarApp::draw()
{
	gl::clear(Color(0.043, 0.161, 0.29));
	gl::enableDepthRead();
	gl::enableDepthWrite();

	gl::setMatrices(mCam);

	player.draw();
}
CINDER_APP(GuitarApp, RendererGl)
