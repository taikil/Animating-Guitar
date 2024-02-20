#include "Player.h"


Player::Player() {
	setup();
}

void Player::setup() {
	handPos = vec3(0, 0, 0);
	auto lambert = gl::ShaderDef().lambert().color();
	gl::GlslProgRef shader = gl::getStockShader(lambert);

	auto capsule = geom::Cube().size(0.6, 0.8, 0.2);
	mRect = gl::Batch::create(capsule, shader);

	for (int i = 0; i < 4; i++) {
		float rel = i / float(4.0);
		for (int j = 0; j < 3; j++) {
			auto knuckle = geom::Capsule().subdivisionsAxis(10)
				.subdivisionsHeight(10).length(fingerLen[i] * knuckleLen[j]).radius(0.1f);
			auto trans = geom::Translate(rel, 0, 0);
			mFingers[i][j] = gl::Batch::create(knuckle >> trans, shader);
		}

	}
}

void Player::update() {
}

void Player::draw() {
	{
		gl::pushModelMatrix();
		//Palm
		gl::color(0.5, 0, 0.1);

		glm::mat4 shearMatrix;
		gl::pushModelMatrix();
		{
			gl::translate(0.1, 0, 0); // Slightly skew the shear
			shearMatrix = glm::mat4(1.0f);
			shearMatrix[1][0] = 0.2f;
			gl::multModelMatrix(shearMatrix);
			mRect->draw();
		}
		gl::popModelMatrix();

		gl::pushModelMatrix();
		{
			gl::translate(-0.1, 0, 0); // Slightly skew the shear
			shearMatrix = glm::mat4(1.0f);
			shearMatrix[1][0] = -0.2f;
			gl::multModelMatrix(shearMatrix);
			mRect->draw();
		}
		gl::popModelMatrix();

		//Fingers
		{

			gl::pushModelMatrix();
			gl::translate(-0.4, 0, 0);
			for (int i = 0; i < 4; i++) {
				gl::pushModelMatrix(); //1x
				{
					gl::translate(0, 1 - fingerLen[i] * knuckleLen[0], 0);
					for (int j = 0; j < 3; j++) {
						gl::pushModelMatrix(); // 3x
						{
							vec3 pivotPoint(0, -knuckleLen[j] * fingerLen[i] + 0.1, 0);
							gl::translate(pivotPoint);
							gl::rotate(angleAxis(float(M_PI / 6) * knuckleLen[j], vec3(1, 0, 0)));
							gl::translate(-pivotPoint);
							mFingers[i][j]->draw();
							gl::translate(0, knuckleLen[j] * fingerLen[i], 0);
						}
					}
				}
				popN(4); //1x + 3x
			}
		}
		popN(2);
	}
}

void Player::popN(int n) {
	for (int i = 0; i < n; i++) {
		gl::popModelMatrix();
	}
}
