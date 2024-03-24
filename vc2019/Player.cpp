#include "Player.h"


Player::Player() {
	setup();
	guitar = GuitarModel();
}

void Player::setup() {
	auto lambert = gl::ShaderDef().lambert().color();
	gl::GlslProgRef shader = gl::getStockShader(lambert);
	auto body = geom::Cube().size(1.8, 3.0, 0.9);
	mBody = gl::Batch::create(body, shader);

	auto sphere = geom::Sphere().subdivisions(40);
	mHead = gl::Batch::create(sphere, shader);
	//auto rotation = geom::Rotate(0, 0, 90);
	auto dot = geom::Sphere().subdivisions(40).radius(0.01);
	sampleDot = gl::Batch::create(dot, shader);
	auto upperArm = geom::Capsule().subdivisionsAxis(10)
		.subdivisionsHeight(10).length(1.0).radius(0.25);
	auto forearm = geom::Capsule().subdivisionsAxis(10)
		.subdivisionsHeight(10).length(1.0).radius(0.25);
	mArmL[0] = gl::Batch::create(upperArm, shader);
	mArmL[1] = gl::Batch::create(forearm, shader);
	mArmR[0] = gl::Batch::create(upperArm, shader);
	mArmR[1] = gl::Batch::create(forearm, shader);
}

void Player::update() {
}

void Player::drawBody() {
	gl::pushModelMatrix();
	{
		gl::color(0.5, 0, 0.5);
		gl::translate(0.1, 0, 0); // Slightly skew the shear
		glm::mat4 shearMatrix = glm::mat4(1.0f);
		shearMatrix[1][0] = 0.1f;
		gl::multModelMatrix(shearMatrix);
		mBody->draw();
	}
	gl::popModelMatrix();

	gl::pushModelMatrix();
	{
		gl::color(0.5, 0, 0.5);
		gl::translate(-0.1, 0, 0); // Slightly skew the shear
		glm::mat4 shearMatrix = glm::mat4(1.0f);
		shearMatrix[1][0] = -0.1f;
		gl::multModelMatrix(shearMatrix);
		mBody->draw();
	}
	gl::popModelMatrix();
}

void Player::drawHands(bool right) {
	gl::pushModelMatrix();
	{
		if (right) {
			rHand.draw();
		}
		else {
			lHand.draw();
		}
	}
	gl::popModelMatrix();
}

void Player::drawHead() {
	gl::pushModelMatrix();
	{
		gl::translate(0, 2.2, 0);
		gl::scale(0.7, 0.8, 0.7);
		mHead->draw();
	}
	gl::popModelMatrix();
}

void Player::drawArms() {

	//Right Arm
	gl::pushModelMatrix();
	{
		gl::translate(-1.1, 0, 0);
		Helpers::rotateFromBase(float(M_PI / 2), vec3(0, 0, 1), vec3(0.666, 0, 0)); // T - pose
		Helpers::rotateFromBase(float(upperArmRRot.x), vec3(1, 0, 0), vec3(0, 0.666, 0));
		gl::rotate(angleAxis(float(upperArmRRot.y), vec3(0, 1, 0)));
		Helpers::rotateFromBase(float(upperArmRRot.z), vec3(0, 0, 1), vec3(0, 0.666, 0));
		gl::pushModelMatrix();
		{
			mArmR[0]->draw();
		}
		gl::popModelMatrix();
		gl::pushModelMatrix();
		{
			gl::translate(0, 1.333, 0);
			Helpers::rotateFromBase(float(forarmRRot.x), vec3(1, 0, 0), vec3(0, 0.666, 0));
			gl::rotate(angleAxis(float(forarmRRot.y), vec3(0, 1, 0)));
			Helpers::rotateFromBase(float(forarmRRot.z), vec3(0, 0, 1), vec3(0, 0.666, 0));
			mArmR[1]->draw();
			gl::pushModelMatrix();
			{
				gl::translate(0, 1, 0);
				drawHands(true);
			}
			gl::popModelMatrix();
		}
		gl::popModelMatrix();
	}
	gl::popModelMatrix();

	//Left Arm
	gl::pushModelMatrix();
	{
		gl::translate(1.1, 0, 0);
		Helpers::rotateFromBase(float(-M_PI / 2), vec3(0, 0, 1), vec3(-0.666, 0, 0)); // T - pose
		Helpers::rotateFromBase(float(upperArmLRot.x), vec3(1, 0, 0), vec3(0, 0.666, 0));
		gl::rotate(angleAxis(float(upperArmLRot.y), vec3(0, 1, 0)));
		Helpers::rotateFromBase(float(upperArmLRot.z), vec3(0, 0, 1), vec3(0, 0.666, 0));
		gl::pushModelMatrix();
		{
			mArmL[0]->draw();
		}
		gl::popModelMatrix();
		gl::pushModelMatrix();
		{
			gl::translate(0, 1.333, 0);
			Helpers::rotateFromBase(float(forarmLRot.x), vec3(1, 0, 0), vec3(0, 0.666, 0));
			gl::rotate(angleAxis(float(forarmLRot.y), vec3(0, 1, 0)));
			Helpers::rotateFromBase(float(forarmLRot.z), vec3(0, 0, 1), vec3(0, 0.666, 0));
			mArmL[1]->draw();
			gl::pushModelMatrix();
			{
				gl::translate(0, 0.9, 0);
				drawHands(false);
			}
			gl::popModelMatrix();
		}
		gl::popModelMatrix();
	}
	gl::popModelMatrix();
}

std::vector<glm::vec3>fabrik(std::vector<glm::vec3>& joint_positions, const glm::vec3& target_position, const std::vector<float>& distances, float tolerance = 0.01f) {
	int n = joint_positions.size();
	float dist = glm::length(joint_positions[0] - target_position);

	if (dist > std::accumulate(distances.begin(), distances.end(), 0.0f) + tolerance) {
		for (int i = 0; i < n - 1; i++) {
			float ri = glm::length(target_position - joint_positions[i]);
			float lambdai = distances[i] / ri;
			joint_positions[i + 1] = (1.0f - lambdai) * joint_positions[i] + lambdai * target_position;
		}
	}
	else {
		glm::vec3 b = joint_positions[0];
		float difA = glm::length(joint_positions.back() - target_position);
		while (difA > tolerance) {
			// Stage 1: Forward Reaching
			joint_positions.back() = target_position;
			for (int i = n - 1; i > 0; i--) {
				float ri = glm::length(joint_positions[i + 1] - joint_positions[i]);
				float lambdai = distances[i] / ri;
				joint_positions[i] = (1.0f - lambdai) * joint_positions[i + 1] + lambdai * joint_positions[i];
			}

			// Stage 2: Backward Reaching
			joint_positions[0] = b;
			for (int i = 0; i < n - 1; i++) {
				float ri = glm::length(joint_positions[i + 1] - joint_positions[i]);
				float lambdai = distances[i] / ri;
				joint_positions[i + 1] = (1.0f - lambdai) * joint_positions[i] + lambdai * joint_positions[i + 1];
			}

			difA = glm::length(joint_positions.back() - target_position);
		}
	}

	return joint_positions;
}

void Player::draw() {
	//CI_LOG_V("MyApp draw() function");

	gl::pushModelMatrix();
	{
		//Rotata guitar, keep in world co-ords
		gl::rotate(angleAxis(float(-M_PI / 4), vec3(0, 0, 1)));
		gl::pushModelMatrix();
		{

			gl::rotate(angleAxis(float(M_PI / 4), vec3(0, 0, 1)));
			drawBody();
			drawHead();
			drawArms();
		}
		gl::popModelMatrix();
		gl::pushModelMatrix();
		gl::translate(0, 1, 1);
		guitar.draw();
		gl::popModelMatrix();

		//Dots to get fretting positions
		gl::pushModelMatrix();
		{
			gl::translate(-0.06, 3.05, 1.27);
			float stringDistance = 0.04;
			float fretDistance = -0.12;
			float depthZ = 0;
			for (int i = 0; i < 21; i++) {
				for (int j = 0; j < 6; j++) {
					gl::pushModelMatrix();
					sampleDot->draw();
					gl::translate(stringDistance, 0, depthZ);
				}
				popN(6);
				gl::pushModelMatrix();
				stringDistance *= 1.024;
				fretDistance += 0.0002;
				depthZ += 0.00001;
				gl::translate(-0.0025, fretDistance, 0);
			}
			popN(21);
		}
		gl::popModelMatrix();
	}
	gl::popModelMatrix();
}
void Player::popN(int n) {
	for (int i = 0; i < n; i++) {
		gl::popModelMatrix();
	}
}
