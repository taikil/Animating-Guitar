#include "Player.h"


Player::Player() {
	setup();
}

void Player::setup() {
	auto lambert = gl::ShaderDef().lambert().color();
	gl::GlslProgRef shader = gl::getStockShader(lambert);
	auto sphere = geom::Sphere().subdivisions(30);
	mSphere = gl::Batch::create(sphere, shader);
}

void Player::update() {
}

void Player::draw() {
	gl::pushModelMatrix();


	gl::translate(2, 0, 0);
	lHand.draw();
	gl::popModelMatrix();
	gl::pushModelMatrix();
	gl::translate(-2, 0, 0);
	rHand.draw();
	gl::popModelMatrix();

	gl::pushModelMatrix();
	gl::color(0.5, 0, 0.5);
	gl::scale(0.8, 2, 0.8); // hand shape
	mSphere->draw();
	gl::popModelMatrix();

	gl::pushModelMatrix();
	gl::translate(0, 2, 0);
	mSphere->draw();
	gl::popModelMatrix();
}
void Player::popN(int n) {
	for (int i = 0; i < n; i++) {
		gl::popModelMatrix();
	}
}
