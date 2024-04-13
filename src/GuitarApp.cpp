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
	void keyDown(ci::app::KeyEvent event) override;


	static const int NUM_SLICES = 12;

	CameraPersp		mCam;
	gl::BatchRef	mSlices[NUM_SLICES];

private:
	MusicSimulator mMusicSim;
	Player player = Player();
	vec2 mInitialMousePos;
	quat mRotationQuat;
	vec3 eyePos = vec3(0, 0, 5);
	vec3 currentTarget;
	bool paused = true;
	int count = 0;
	int string = 0;
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
	currentTarget = player.getCurrentNotePos(0, 0);
	//player.IKSolver(false, currentTarget);
	//player.IKSolver(true, vec3(0.5, 0.5, 1.5));
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
		float zoomSpeed = 0.01f;
		eyePos.z += mouseDelta.y * zoomSpeed;

		// Ensure that the camera does not go too close or too far
		eyePos.z = math<float>::clamp(eyePos.z, -20.0f, -1.0f);

		// Update the camera's lookAt and eye positions
		mCam.lookAt(eyePos, vec3(0));
	}
}

void GuitarApp::keyDown(ci::app::KeyEvent event) {
	if (event.getCode() == ci::app::KeyEvent::KEY_s) {
		paused = !paused; // Toggle pause state
	}
}

void GuitarApp::update()
{
	if (count == 20) count = 0;
	if (string == 5) {
		string = 0;
		count++;
	}
	string++;
	if (!paused) {
		currentTarget = player.getCurrentNotePos(count, string);
		player.IKSolver(false, currentTarget);
		player.IKSolver(true, vec3(0.5, 0.5, 1.5));
	}
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
