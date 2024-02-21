#include "Hand.h"


Hand::Hand(bool rightHand) {
	this->rightHand = rightHand;
	setup();
}

void Hand::setup() {
	handPos = vec3(0, 0, 0);
	auto lambert = gl::ShaderDef().lambert().color();
	gl::GlslProgRef shader = gl::getStockShader(lambert);

	auto capsule = geom::Cube().size(0.6, 0.8, 0.2);
	mRect = gl::Batch::create(capsule, shader);

	//4 fingers
	for (int i = 0; i < 4; i++) {
		float rel = i / float(4.0);
		for (int j = 0; j < 3; j++) {
			auto knuckle = geom::Capsule().subdivisionsAxis(10)
				.subdivisionsHeight(10).length(fingerLen[i] * knuckleLen[j]).radius(0.1f);
			auto trans = geom::Translate(rel, 0, 0);
			mFingers[i][j] = gl::Batch::create(knuckle >> trans, shader);
		}
	}
	//Thumb
	auto thumb0 = geom::Sphere().subdivisions(10).radius(0.12f);
	mThumb[0] = gl::Batch::create(thumb0, shader);
	for (int i = 1; i < 3; i++) {
		auto thumb = geom::Capsule().subdivisionsAxis(10).subdivisionsHeight(10).length(thumbKnuckleLen[i] * thumbLen).radius(0.12f);
		mThumb[i] = gl::Batch::create(thumb, shader);
	}
}

void Hand::update() {
}

void Hand::draw() {
	{
		gl::pushModelMatrix();
		//Palm
		if (rightHand) {
			gl::color(0.1, 0, 0.5);
		}
		else {
			gl::color(0.5, 0, 0.1);
		}

		if (rightHand) {
			gl::scale(-1, 1, 1);  // Mirroring about the y-axis
		}
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
			gl::translate(-0.2, 0, 0); // Slightly skew the shear
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
					gl::translate(0, 1 - knuckleLen[0], 0);
					for (int j = 0; j < 3; j++) {
						gl::pushModelMatrix(); // 3x
						{
							Helpers::rotateFromBase(float(M_PI / 4) * knuckleLen[j], vec3(1, 0, 0), vec3(0, 0.5 * (knuckleLen[j] * fingerLen[i] - 0.1), 0));
							mFingers[i][j]->draw();
							gl::translate(0, knuckleLen[j] * fingerLen[i], 0);
						}
					}
				}
				popN(4); //1x + 3x
			}
		}
		popN(2);
		//Thumb
		gl::pushModelMatrix();
		{
			gl::pushModelMatrix(); // Base of thumb
			{
				vec3 thumbTrans = (rightHand) ? vec3(0.4, 0, 0) : vec3(-0.4, 0, 0);
				gl::translate(thumbTrans);
				float thumbRotation = (rightHand) ? -(M_PI / 3) : (M_PI / 3);
				Helpers::rotateFromBase(thumbRotation, vec3(0, 1, 1), vec3(0, 0, 0));
				gl::scale(1.2, 2, 1.2);
				mThumb[0]->draw();
			}
			gl::popModelMatrix();

			vec3 thumbTrans = (rightHand) ? vec3(0.4, 0.2, 0) : vec3(-0.4, 0.2, 0);
			gl::translate(thumbTrans);
			float thumbJointRotation = (rightHand) ? -(M_PI / 3) : (M_PI / 3);
			Helpers::rotateFromBase(thumbJointRotation, vec3(0, 0, 1), vec3(0, 0.25, 0));
			float thumbTipRotation = (rightHand) ? -(M_PI / 6) : (M_PI / 6);
			Helpers::rotateFromBase((M_PI / 6), vec3(1, 0, 0), vec3(0, 0.25, 0));
			mThumb[1]->draw();
			gl::pushModelMatrix();
			{
				gl::translate(0, 0.4, 0);
				Helpers::rotateFromBase((M_PI / 6), vec3(1, 0, 0), vec3(0, 0.25, 0));
				mThumb[2]->draw();
			}
			gl::popModelMatrix();

		}
		gl::popModelMatrix();
	}
}

void Hand::popN(int n) {
	for (int i = 0; i < n; i++) {
		gl::popModelMatrix();
	}
}
