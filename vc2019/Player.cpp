#include "Player.h"


Player::Player() {
	setup();
}

void Player::setup() {
	auto lambert = gl::ShaderDef().lambert().color();
	gl::GlslProgRef shader = gl::getStockShader(lambert);
	auto body = geom::Cube().size(1.8, 3.0, 0.9);
	mBody = gl::Batch::create(body, shader);

	auto sphere = geom::Sphere().subdivisions(40);
	mHead = gl::Batch::create(sphere, shader);
	//auto rotation = geom::Rotate(0, 0, 90);
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
			Helpers::rotateFromBase(float(M_PI / 6), vec3(1, 0, 0), vec3(0, 0.666, 0));
			Helpers::rotateFromBase(float(M_PI / 12), vec3(0, 1, 0), vec3(0, 0, 0));
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
			Helpers::rotateFromBase(float(upperArmLRot.x), vec3(1, 0, 0), vec3(0, 0.666, 0));
			gl::rotate(angleAxis(float(upperArmLRot.y), vec3(0, 1, 0)));
			Helpers::rotateFromBase(float(upperArmLRot.z), vec3(0, 0, 1), vec3(0, 0.666, 0));
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

void Player::draw() {
	gl::pushModelMatrix();
	{
		//drawHands();
		drawBody();
		drawHead();
		drawArms();


	}
	gl::popModelMatrix();
}
void Player::popN(int n) {
	for (int i = 0; i < n; i++) {
		gl::popModelMatrix();
	}
}
