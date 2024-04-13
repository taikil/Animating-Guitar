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
	void drawFrets();
	void allRotations(vec3 thetas, vec3 distance);


	std::vector<vec3> computeJointRotations(const std::vector<glm::vec3>& joint_positions);
	std::vector<vec3> computeJointPositions(const std::vector<vec3>& translations, const std::vector<float>& distances, const std::vector<vec3>& rotations);
	vec3 getCurrentNotePos();
	void IKSolver(bool right, const vec3& target_position);
	std::vector<vec3> fabrik(std::vector<vec3>& joint_positions, const vec3& target_position, const std::vector<float>& distances, float tolerance = 0.01f);

	gl::BatchRef	sampleDot;
	gl::BatchRef	mBody;
	gl::BatchRef	mHead;
	gl::BatchRef	mArm;

private:
	GuitarModel guitar;

	std::vector<vec3> notes;
	Hand lHand;
	Hand rHand;
	// x y z rotation
	// [ RShoulder, RForearm, RWrist, LShoulder, LForearm, LWrist ]
	std::vector<vec3> armTranslationsR = { vec3(-1.1, 0, 0), vec3(0, 1.333, 0), vec3(0, 0.9, 0) };
	std::vector<vec3> armTranslationsL = { vec3(1.1, 0, 0), vec3(0, 1.333, 0), vec3(0, 0.9, 0) };
	std::vector<vec3> armPositionR = { vec3(-1.1, 0.666, 0), vec3(-2.4333, 0.666, 0), vec3(-3.7666, 0.666, 0) };
	std::vector<vec3> armPositionL = { vec3(1.1, 0.666, 0), vec3(2.4333, 0.666, 0), vec3(3.7666, 0.666, 0) };
	// TODO Return position of each joint!!!
	std::vector<float> distances = { 1.333, 1.333, 0.9 };
	std::vector<vec3> armRotationsR;
	std::vector<vec3> armRotationsL;
	float armRootRotation = M_PI / 2;
	//Finger names: Index: i, Middle: m, Ring: r, Pinky: p, Thumb: t
	std::vector<float> fingerLengths = { 0.416f, 0.416f, 0.208f, 0.52f, 0.52f, 0.26f, 0.468f, 0.468f, 0.234f, 0.338f, 0.338f, 0.169f }; // Each finger segment lens * 1.3 (capsule len)
	std::vector<float> fingerAngles = { M_PI / 12, M_PI / 12, M_PI / 12, M_PI / 12, M_PI / 12, M_PI / 12, M_PI / 12, M_PI / 12, M_PI / 12, M_PI / 12, M_PI / 12, M_PI / 12 };

	bool printedTranslation = false; 

	vec3 thumbKnuckleLen = vec3(0.1, 0.4, 0.3);
	float thumbLen = 0.7;

	// Each fret position
	const vec3 fretPositions[21][6] = {
	{vec3(2.407, 2.492, 1.270), vec3(2.435, 2.464, 1.270), vec3(2.464, 2.435, 1.270),
	 vec3(2.492, 2.407, 1.270), vec3(2.520, 2.379, 1.270), vec3(2.549, 2.351, 1.270)},
	{vec3(2.321, 2.409, 1.270), vec3(2.350, 2.380, 1.270), vec3(2.379, 2.351, 1.270),
	 vec3(2.408, 2.322, 1.270), vec3(2.437, 2.293, 1.270), vec3(2.465, 2.264, 1.270)},
	{vec3(2.234, 2.326, 1.270), vec3(2.264, 2.297, 1.270), vec3(2.294, 2.267, 1.270),
	 vec3(2.323, 2.237, 1.270), vec3(2.353, 2.208, 1.270), vec3(2.383, 2.178, 1.270)},
	{vec3(2.148, 2.244, 1.270), vec3(2.178, 2.213, 1.270), vec3(2.209, 2.183, 1.270),
	 vec3(2.239, 2.152, 1.270), vec3(2.270, 2.122, 1.270), vec3(2.300, 2.092, 1.270)},
	{vec3(2.062, 2.161, 1.270), vec3(2.093, 2.130, 1.270), vec3(2.124, 2.099, 1.270),
	 vec3(2.155, 2.068, 1.270), vec3(2.186, 2.037, 1.270), vec3(2.218, 2.006, 1.270)},
	{vec3(1.976, 2.079, 1.270), vec3(2.008, 2.047, 1.270), vec3(2.040, 2.015, 1.270),
	 vec3(2.072, 1.983, 1.270), vec3(2.104, 1.951, 1.270), vec3(2.135, 1.919, 1.270)},
	{vec3(1.890, 1.996, 1.270), vec3(1.923, 1.964, 1.270), vec3(1.956, 1.931, 1.270),
	 vec3(1.988, 1.899, 1.270), vec3(2.021, 1.866, 1.270), vec3(2.053, 1.833, 1.270)},
	{vec3(1.805, 1.914, 1.270), vec3(1.838, 1.881, 1.270), vec3(1.872, 1.848, 1.270),
	 vec3(1.905, 1.814, 1.270), vec3(1.938, 1.781, 1.270), vec3(1.972, 1.747, 1.270)},
	{vec3(1.719, 1.832, 1.270), vec3(1.753, 1.798, 1.270), vec3(1.788, 1.764, 1.270),
	 vec3(1.822, 1.730, 1.270), vec3(1.856, 1.696, 1.270), vec3(1.890, 1.661, 1.270)},
	{vec3(1.634, 1.751, 1.270), vec3(1.669, 1.716, 1.270), vec3(1.704, 1.681, 1.270),
	 vec3(1.739, 1.646, 1.270), vec3(1.774, 1.611, 1.270), vec3(1.809, 1.576, 1.270)},
	{vec3(1.549, 1.669, 1.270), vec3(1.585, 1.633, 1.270), vec3(1.620, 1.597, 1.270),
	 vec3(1.656, 1.561, 1.270), vec3(1.692, 1.526, 1.270), vec3(1.728, 1.490, 1.271)},
	{vec3(1.464, 1.587, 1.270), vec3(1.500, 1.551, 1.270), vec3(1.537, 1.514, 1.270),
	 vec3(1.574, 1.477, 1.270), vec3(1.611, 1.441, 1.270), vec3(1.647, 1.404, 1.271)},
	{vec3(1.379, 1.506, 1.270), vec3(1.416, 1.468, 1.270), vec3(1.454, 1.431, 1.270),
	 vec3(1.492, 1.393, 1.270), vec3(1.529, 1.356, 1.270), vec3(1.567, 1.318, 1.271)},
	{vec3(1.294, 1.425, 1.270), vec3(1.332, 1.386, 1.270), vec3(1.371, 1.348, 1.270),
	 vec3(1.409, 1.309, 1.270), vec3(1.448, 1.271, 1.271), vec3(1.486, 1.232, 1.271)},
	{vec3(1.209, 1.344, 1.270), vec3(1.249, 1.304, 1.270), vec3(1.288, 1.265, 1.270),
	 vec3(1.328, 1.225, 1.270), vec3(1.367, 1.186, 1.271), vec3(1.406, 1.147, 1.271)},
	{vec3(1.125, 1.263, 1.270), vec3(1.165, 1.222, 1.270), vec3(1.206, 1.182, 1.270),
	 vec3(1.246, 1.142, 1.270), vec3(1.286, 1.101, 1.271), vec3(1.327, 1.061, 1.271)},
	{vec3(1.040, 1.182, 1.270), vec3(1.082, 1.141, 1.270), vec3(1.123, 1.099, 1.270),
	 vec3(1.164, 1.058, 1.270), vec3(1.206, 1.017, 1.271), vec3(1.247, 0.975, 1.271)},
	{vec3(0.956, 1.101, 1.270), vec3(0.999, 1.059, 1.270), vec3(1.041, 1.017, 1.270),
	 vec3(1.083, 0.974, 1.271), vec3(1.126, 0.932, 1.271), vec3(1.168, 0.890, 1.271)},
	{vec3(0.872, 1.021, 1.270), vec3(0.916, 0.977, 1.270), vec3(0.959, 0.934, 1.270),
	 vec3(1.002, 0.891, 1.271), vec3(1.046, 0.847, 1.271), vec3(1.089, 0.804, 1.271)},
	{vec3(0.788, 0.940, 1.270), vec3(0.833, 0.896, 1.270), vec3(0.877, 0.851, 1.270),
	 vec3(0.921, 0.807, 1.271), vec3(0.966, 0.763, 1.271), vec3(1.010, 0.718, 1.271)},
	{vec3(0.704, 0.860, 1.270), vec3(0.750, 0.815, 1.270), vec3(0.795, 0.769, 1.270),
	 vec3(0.841, 0.724, 1.271), vec3(0.886, 0.678, 1.271), vec3(0.932, 0.633, 1.271)}
	};

};

