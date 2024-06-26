#include "Hand.h"

Hand::Hand() {
	this->rightHand = false;
	std::vector<float> angles(12, 0.0f);
	setAngles(angles);
	setup();
}

Hand::Hand(bool rightHand, std::vector<float> angles) {
	this->rightHand = rightHand;
	setAngles(angles);
	setup();
}

void Hand::setup() {
	handPos = vec3(0, 0, 0);
	auto lambert = gl::ShaderDef().lambert().color();
	gl::GlslProgRef shader = gl::getStockShader(lambert);

	auto palm = geom::Cube().size(0.6, 0.8, 0.2);
	mRect = gl::Batch::create(palm, shader);

	//4 fingers
	for (int i = 0; i < 4; i++) {
		float rel = (i / float(4.0));
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

	auto dot = geom::Sphere().subdivisions(40).radius(0.05);
	sampleDot = gl::Batch::create(dot, shader);

	//Switch for right hand
	(rightHand) ? thumbBaseTranslate.x = -thumbBaseTranslate.x : void();
	(rightHand) ? thumbBaseRotation = -thumbBaseRotation : void();
	(rightHand) ? thumbJointTranslate.x = -thumbJointTranslate.x : void();
	(rightHand) ? thumbJointRotation = -thumbJointRotation : void();
}

void Hand::update() {
}

void Hand::setAngles(std::vector<float> angles) {
	int k = 0;
	if (angles.size() == 12) {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 3; j++) {
				fingerFlexion[i][j] = angles[k++];
			}
		}
	}
}

void Hand::draw() {
	gl::pushModelMatrix();
	{
		//Palm

		//gl::scale(1, -1, 1);
		if (rightHand) {
			gl::color(0.1, 0, 0.5);
		}
		else {
			gl::color(0.5, 0, 0.1);
		}

		if (rightHand) {
			//gl::scale(-1, 1, 1);  // Mirroring about the y-axis
			gl::scale(-0.5, 0.5, 0.5);  // Mirroring about the y-axis
		}
		else {
			gl::scale(0.5, 0.5, 0.5);
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
		gl::pushModelMatrix();
		{
			gl::translate(-0.4, 0, 0);
			for (int i = 0; i < 4; i++) {
				gl::pushModelMatrix(); //1x
				{
					gl::translate(0, 1 - knuckleLen[0], 0);
					for (int j = 0; j < 3; j++) {
						gl::pushModelMatrix(); // 3x
						{
							if (j != 0) {
								//double translation = (knuckleLen[j] * fingerLen[i] * 0.666) + (knuckleLen[j - 1] * fingerLen[i - 1] * 0.666);
								gl::translate(0, knuckleLen[j] * fingerLen[i] * 0.666, 0);
								//gl::translate(0, translation, 0);
							}
							Helpers::rotateFromBase(fingerFlexion[i][j], vec3(1, 0, 0), vec3(0, 0.666 * (knuckleLen[j] * fingerLen[i]), 0));
							mFingers[i][j]->draw();
							if (j != 2) {
								gl::translate(0, knuckleLen[j] * fingerLen[i] * 0.666, 0);
							}
						}
					}
				}
				popN(4); //1x + 3x
			}
		}
		gl::popModelMatrix();

		//Thumb
		gl::pushModelMatrix();
		{
			if (rightHand) {
				gl::scale(-1, 1, 1);  // Mirroring about the y-axis
			}
			gl::pushModelMatrix(); // Base of thumb
			{
				gl::translate(thumbBaseTranslate);
				Helpers::rotateFromBase(thumbBaseRotation, vec3(0, 1, 1), vec3(0, 0, 0));
				gl::scale(1.2, 2, 1.2); // hand shape
				mThumb[0]->draw();
			}
			gl::popModelMatrix();

			gl::translate(thumbJointTranslate);
			Helpers::rotateFromBase(thumbJointRotation, vec3(0, 0, 1), vec3(0, 0.25, 0));
			Helpers::rotateFromBase(thumbTipRotation, vec3(1, 0, 0), vec3(0, 0.25, 0));
			mThumb[1]->draw();
			gl::pushModelMatrix();
			{
				gl::translate(thumbTipTranslate);
				Helpers::rotateFromBase(thumbTipRotation, vec3(1, 0, 0), vec3(0, 0.25, 0));
				mThumb[2]->draw();
			}
			gl::popModelMatrix();
		}
		gl::popModelMatrix();
	}
	gl::popModelMatrix();
}
void Hand::popN(int n) {
	for (int i = 0; i < n; i++) {
		gl::popModelMatrix();
	}
}
