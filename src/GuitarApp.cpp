#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Shader.h"
#include "cinder/Easing.h"
#include "cinder/Camera.h"
#include "cinder/Log.h"

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
	vec3 lerp(const vec3& start, const vec3& end, float t);
	float getNormalizedTime();


	static const int NUM_SLICES = 12;

	CameraPersp		mCam;
	gl::BatchRef	mSlices[NUM_SLICES];

private:
	MusicSimulator mMusicSim = MusicSimulator();;
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
}

void GuitarApp::mouseDown(MouseEvent event)
{
	mInitialMousePos = event.getPos();
}

void GuitarApp::mouseDrag(MouseEvent event) {
	if (event.isRightDown()) {
		vec2 mouseDelta = vec2(event.getPos()) - mInitialMousePos;

		quat rotationQuat = quat(vec3(-mouseDelta.y, -mouseDelta.x, 0) * 0.01f);

		mRotationQuat = rotationQuat * mRotationQuat;

		vec3 eye = mRotationQuat * eyePos;
		vec3 target = vec3(0);
		mCam.lookAt(eye + target, target);
		mInitialMousePos = event.getPos();
	}
	else {
		vec2 mouseDelta = vec2(event.getPos()) - mInitialMousePos;

		float zoomSpeed = 0.01f;
		eyePos.z += mouseDelta.y * zoomSpeed;

		eyePos.z = math<float>::clamp(eyePos.z, -20.0f, -1.0f);

		mCam.lookAt(eyePos, vec3(0));
	}
}

void GuitarApp::keyDown(ci::app::KeyEvent event) {
	if (event.getCode() == ci::app::KeyEvent::KEY_s) {
		paused ? mMusicSim.startSimulation() : mMusicSim.stopSimulation();
		paused = !paused;
	}
}

void GuitarApp::update()
{

	if (!paused) {
		if (count == 20) count = 0;
		if (string == 5) {
			string = 0;
			count++;
		}
		float t = mMusicSim.getTime();
		float normalizedTime = getNormalizedTime();
		CI_LOG_V("Current Time (in seconds): " << t);
		CI_LOG_V("Normalized Time (in seconds): " << normalizedTime);
		vec3 startPositionL = player.getCurrentEndPos(false);
		vec3 startPositionR = player.getCurrentEndPos(true);
		currentTarget = player.getCurrentNotePos(count, string);
		vec3 rightTarget = vec3(0.2, 0.2, 2.0);
		vec3 interpolatedPositionL = lerp(startPositionL, currentTarget,  0.5 * normalizedTime);
		vec3 interpolatedPositionR = lerp(startPositionR, rightTarget, normalizedTime);
		float distanceThreshold = 0.05f;  // Adjust as needed
		float distanceL = glm::length(currentTarget - interpolatedPositionL);
		CI_LOG_V("Distance" << distanceL);
		if (distanceL < distanceThreshold) {
			string++;
		}

		player.IKSolver(false, interpolatedPositionL);
		player.IKSolver(true, interpolatedPositionR);
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

vec3 GuitarApp::lerp(const vec3& start, const vec3& end, float t) {
	return start + t * (end - start);
}

float GuitarApp::getNormalizedTime() {
	float beatTime = mMusicSim.getTimePerBeat();
	float elapsedTime = mMusicSim.getTime();

	float currentBeat = elapsedTime / beatTime;
	float normalizedTime = fmodf(currentBeat, 1.0f);

	const float easeDuration = 0.3f; // Percentage of the beat for easing

	if (normalizedTime < easeDuration) {
		normalizedTime /= easeDuration; // Normalize for easing
		normalizedTime = ci::easeInOutQuad(normalizedTime); // Apply easing
	}
	else if (normalizedTime > 1.0f - easeDuration) {
		normalizedTime -= (1.0f - easeDuration); // Shift for end easing
		normalizedTime /= easeDuration;
		normalizedTime = 1.0f - ci::easeInOutQuad(1.0f - normalizedTime); // Apply easing (reversed)
	}

	return normalizedTime;
}

