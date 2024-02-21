#include "Player.h"


Player::Player() {
	setup();
}

void Player::setup() {
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
}
void Player::popN(int n) {
	for (int i = 0; i < n; i++) {
		gl::popModelMatrix();
	}
}
