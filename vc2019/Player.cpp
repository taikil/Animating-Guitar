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
		.subdivisionsHeight(10).length(1.0);
	auto forearm = geom::Capsule().subdivisionsAxis(10)
		.subdivisionsHeight(10).length(1.0);
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

void Player::drawHands() {
	gl::pushModelMatrix();
	{
		gl::translate(4, 0, 0);
		lHand.draw();
	}
	gl::popModelMatrix();

	gl::pushModelMatrix();
	{
		gl::translate(-4, 0, 0);
		rHand.draw();
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

void Player::draw() {
	gl::pushModelMatrix();
	{
		drawHands();
		drawBody();
		drawHead();

		gl::pushModelMatrix();
		{
			gl::translate(-1, 0, 0);
			Helpers::rotateFromBase(90, vec3(0, 0, 1), vec3(0.5, 0, 0));
			gl::scale(0.5, 1, 0.5);
			mArmL[0]->draw();
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
