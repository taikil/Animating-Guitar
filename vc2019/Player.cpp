#include "Player.h"


Player::Player() {
    setup();
}

void Player::setup() {
    auto lambert = gl::ShaderDef().lambert().color();
    gl::GlslProgRef shader = gl::getStockShader(lambert);

    auto torus = geom::Torus();
    mRect = gl::Batch::create(torus, shader);
}

void Player::update() {
}

void Player::draw() {
    gl::enableDepthRead();
    gl::enableDepthWrite();

    gl::pushModelMatrix();

    gl::color(0.3, 0, 1);
    gl::translate(0, 2, 0);
    mRect->draw();
    
    gl::popModelMatrix();
}
