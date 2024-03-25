#pragma once

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "Helpers.h"
#include "Hand.h"
#include "GuitarModel.h"
#include "cinder/Log.h"
#include <vector>
#include <numeric>

using namespace ci;

struct Joint {
	vec3 position; // Position of the joint
	float length; // Length of the segment connecting it to the parent joint
	vec3 angles; // Angles of rotation around each axis
};

class Player
{
public:
	Player();
	void setup();
	void update();
	void draw();
	void popN(int n);
	void drawBody();
	void drawHead();
	void drawArms();
	void drawHands(bool right);

	std::vector<glm::vec3> getJointPositions(std::vector<glm::vec3> joint_positions);
	std::vector<glm::vec3> fabrik(std::vector<glm::vec3>& joint_positions, const glm::vec3& target_position, const std::vector<float>& distances, float tolerance = 0.01f);

	gl::BatchRef	sampleDot;

	gl::BatchRef	mBody;
	gl::BatchRef	mHead;
	gl::BatchRef	mArmR[2];
	gl::BatchRef	mArmL[2];
	gl::BatchRef	mLegs[4];

private:
	GuitarModel guitar;
	vec3 stringPosition[21][6];
	Hand lHand = Hand(false, fingerAngles);
	Hand rHand = Hand(true, fingerAngles);
	// x y z rotation
	vec3 upperArmLRot = vec3(M_PI / 4, 0, -M_PI / 6);
	vec3 forarmLRot = vec3(M_PI / 2, M_PI / 12, M_PI / 6);
	vec3 upperArmRRot = vec3(M_PI / 6, 0, M_PI / 6);
	vec3 forarmRRot = vec3(M_PI / 2, 0, 0);
	//Finger names: Index: i, Middle: m, Ring: r, Pinky: p, Thumb: t
	std::vector<float> fingerLengths = { 0.416f, 0.416f, 0.208f, 0.52f, 0.52f, 0.26f, 0.468f, 0.468f, 0.234f, 0.338f, 0.338f, 0.169f }; // Each finger segment lens * 1.3 (capsule len)
	std::vector<float> fingerAngles = { M_PI / 4, M_PI / 4, M_PI / 4, M_PI / 4, M_PI / 4, M_PI / 4, M_PI / 4, M_PI / 4, M_PI / 4, M_PI / 4, M_PI / 4, M_PI / 4 };
	std::vector<Joint> rHandJoints;
	std::vector<Joint> lHandJoints;
	std::vector<Joint> rArmJoints;
	std::vector<Joint> lArmJoints;


	vec4 fingerLen = vec4(0.9, 1.0, 0.95, 0.7);
	vec3 knuckleLen = vec3(0.4, 0.4, 0.2);
	vec3 thumbKnuckleLen = vec3(0.1, 0.4, 0.3);
	float thumbLen = 0.7;
};

