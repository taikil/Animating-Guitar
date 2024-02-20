#include "Player.h"


Player::Player() {
	setup();
}

void Player::setup() {
	handPos = vec3(0, 0, 0);
	auto lambert = gl::ShaderDef().lambert().color();
	gl::GlslProgRef shader = gl::getStockShader(lambert);

	auto torus = geom::Cube().size(1, 1, 0.2);
	mRect = gl::Batch::create(torus, shader);
}

void Player::update() {
}

void Player::draw() {

	gl::pushModelMatrix();

	gl::color(0.5, 0, 0.1);
	gl::translate(0, 0, 0);
	float angle = M_PI / 6.0;
	gl::rotate(angleAxis(angle, vec3(0, 1, 0)));
	mRect->draw();

	gl::popModelMatrix();
}
