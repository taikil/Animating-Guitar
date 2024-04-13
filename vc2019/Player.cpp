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


	auto arm = geom::Capsule().subdivisionsAxis(10)
		.subdivisionsHeight(10).length(1.0).radius(0.25);

	auto rot = geom::Rotate(M_PI / 2, vec3(0, 0, 1));

	mArm = gl::Batch::create(arm, shader);

	lHand = Hand(false, fingerAngles);
	rHand = Hand(true, fingerAngles);
	armRotationsR.resize(3, glm::vec3(0, 0, 0));
	armRotationsL.resize(3, glm::vec3(0, 0, 0));
	//armRotationsR.resize(3, glm::vec3(M_PI / 12, M_PI / 12, 0));
	//armRotationsL.resize(3, glm::vec3(M_PI / 12, -M_PI / 12, 0));
	//armPositionR = computeJointPositions(armTranslationsR, distances, armRotationsR);
	//armPositionL = computeJointPositions(armTranslationsL, distances, armRotationsL);
	//CI_LOG_V("shoulder: (" << armPositionL[0].x << ", " << armPositionL[0].y << ", " << armPositionL[0].z << ")");
	//CI_LOG_V("forearm: (" << armPositionL[1].x << ", " << armPositionL[1].y << ", " << armPositionL[1].z << ")");
	//CI_LOG_V("hand: (" << armPositionL[2].x << ", " << armPositionL[2].y << ", " << armPositionL[2].z << ")");

}

void Player::update() {
}

vec3 Player::getCurrentNotePos(int a, int b) {
	return fretPositions[a][b];
	//return vec3(1.669, 1.716, 1.270);
}

vec3 Player::getCurrentEndPos(bool right) {
	return right ? armPositionR.back() : armPositionL.back();
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

	//Right Arm i = 0
	for (int i = 0; i < 2; i++) {
		std::vector<vec3> translation = i == 0 ? armPositionR : armPositionL;
		//std::vector<vec3> rotations = i == 0 ? armRotationsR : armRotationsL;
		gl::pushModelMatrix();
		{
			for (int j = 0; j < translation.size() - 1; j++) {
				gl::ScopedLineWidth lineWidth(10.0f);
				gl::drawLine(translation[j], translation[j + 1]);
			}
			//	gl::translate(translation[0]);
			//	Helpers::rotateFromBase(i == 0 ? armRootRotation : -armRootRotation,
			//		vec3(0, 0, 1), vec3(i == 0 ? 0.666 : -0.666, 0, 0)); // T - pose rotation from root
			//	allRotations(rotations[0], vec3(0, 0.666, 0));
			//	mArm->draw(); //Shoulder
			//	gl::pushModelMatrix();
			//	{
			//		gl::translate(translation[1]);
			//		allRotations(rotations[1], vec3(0, 0.666, 0));
			//		mArm->draw(); // Forearm
			//		gl::pushModelMatrix();
			//		{
			//			gl::translate(translation[2]);
			//			allRotations(rotations[2], vec3(0, 0.4, 0));
			//			drawHands(i == 0 ? true : false);
			//		}
			//		gl::popModelMatrix();
			//	}
			//	gl::popModelMatrix();
		}
		gl::popModelMatrix();
	}
}

void Player::allRotations(vec3 thetas, vec3 distance) {
	//thetas *= 180 / M_PI;
	Helpers::rotateFromBase(float(thetas.x), vec3(1, 0, 0), distance);
	gl::rotate(angleAxis(float(thetas.y), vec3(0, 1, 0)));
	Helpers::rotateFromBase(float(thetas.z), vec3(0, 0, 1), distance);
}

void Player::IKSolver(bool right, const vec3& target_position) {
	std::vector<vec3> newPos;
	std::vector<vec3> newThetas;
	if (right) {
		if (glm::length(armPositionR.back() - target_position) < 0.01f) return;

		newPos = fabrik(armPositionR, target_position, distances);
		newThetas = computeJointRotations(newPos);
		vec3 temp = armRotationsR.back();
		//newThetas[0].y = -newThetas[0].y;
		newThetas[1].y = 0;

		armRotationsR = newThetas;
		armRotationsR.push_back(temp);
	}
	else {
		if (glm::length(armPositionL.back() - target_position) < 0.01f) return;

		newPos = fabrik(armPositionL, target_position, distances);
		newThetas = computeJointRotations(newPos);
		vec3 temp = armRotationsL.back();
		//newThetas[0].x = -newThetas[0].x;
		//newThetas[1].x = -newThetas[1].x;
		//newThetas[0].y = -newThetas[0].y;
		newThetas[1].y = 0;
		newThetas[0].z = -newThetas[0].z;
		newThetas[1].z = -newThetas[1].z;
		armRotationsL = newThetas;
		armRotationsL.push_back(temp);
	}

}

std::vector<vec3> Player::fabrik(std::vector<vec3>& p, const vec3& t, const std::vector<float>& d, float tol) {
	int n = p.size() - 1;
	float dist = glm::length(p[0] - t);

	if (dist > std::accumulate(d.begin(), d.end(), 0.0f) + tol) {
		for (int i = 0; i < n - 1; i++) {
			float ri = glm::length(t - p[i]);
			float lambdai = d[i] / ri;
			p[i + 1] = (1.0f - lambdai) * p[i] + lambdai * t;
		}
		CI_LOG_V("UNREACHABLE!");
	}
	else {
		vec3 b = p[0];
		float difA = glm::length(p.back() - t);
		while (difA > tol) {
			// Stage 1: Forward Reaching
			p.back() = t;
			for (int i = n - 1; i > 0; i--) {
				float ri = glm::length(p[i + 1] - p[i]);
				float lambdai = d[i] / ri;
				p[i] = (1.0f - lambdai) * p[i + 1] + lambdai * p[i];
			}

			// Stage 2: Backward Reaching
			p[0] = b;
			for (int i = 0; i < n - 1; i++) {
				float ri = glm::length(p[i + 1] - p[i]);
				float lambdai = d[i] / ri;
				p[i + 1] = (1.0f - lambdai) * p[i] + lambdai * p[i + 1];
			}

			difA = glm::length(p.back() - t);
		}
	}
	return p;
}

std::vector<glm::vec3> Player::computeJointRotations(const std::vector<glm::vec3>& joint_positions) {
	std::vector<glm::vec3> joint_rotations;

	// Iterate over each joint position
	for (size_t i = 0; i < joint_positions.size() - 1; ++i) {
		// Calculate the direction vector between the current joint and the next joint
		glm::vec3 direction = glm::normalize(joint_positions[i + 1] - joint_positions[i]);

		// Calculate the rotation angles using trigonometry
		float y = std::atan2(direction.y, direction.z); //swapped because of earlier rotation
		float x = std::atan2(direction.x, direction.z);
		float z = std::atan2(direction.x, direction.y);

		joint_rotations.push_back(glm::vec3(x, y, z));
	}

	for (int i = joint_rotations.size() - 1; i > 0; i--) {
		joint_rotations[i] -= joint_rotations[i - 1]; // get into it's own coordinates
	}

	return joint_rotations;
}

std::vector<vec3> Player::computeJointPositions(const std::vector<vec3>& translations, const std::vector<float>& distances, const std::vector<vec3>& rotations)
{
	std::vector<vec3> jointPositions;
	jointPositions.reserve(translations.size());

	vec3 prevPosition = vec3(0, 0, 0);
	//jointPositions.push_back(currentPosition);

	for (size_t i = 0; i < translations.size(); ++i) {
		// Compute the position of the current joint
		vec3 currentPosition = prevPosition + rotations[i] * vec3(0, distances[i], 0) + translations[i];
		jointPositions.push_back(currentPosition);

		// Update previous position for the next joint
		prevPosition = currentPosition;
	}

	return jointPositions;
}

void Player::drawFrets() {
	//Dots to get fretting positions
	gl::pushModelMatrix();
	{
		gl::translate(-0.06, 2.05, 0.27);
		float stringDistance = 0.04;
		float fretDistance = -0.12;
		float depthZ = 0;
		for (int i = 0; i < 21; i++) {
			for (int j = 0; j < 6; j++) {
				gl::pushModelMatrix();
				sampleDot->draw();
				if (!printedTranslation) {
					vec3 currentTranslation = gl::getModelMatrix() * vec4(0, 0, 0, 1);
					//CI_LOG_V("String: " << i + 1 << ", Translation:" << currentTranslation);
				}
				gl::translate(stringDistance, 0, depthZ);
			}
			popN(6);
			gl::pushModelMatrix();
			stringDistance *= 1.024;
			fretDistance += 0.0002;
			depthZ += 0.00001;
			gl::translate(-0.0025, fretDistance, 0);
		}

		printedTranslation = true;
		popN(21);
	}
	gl::popModelMatrix();
}


void Player::draw() {
	//CI_LOG_V("MyApp draw() function");

	gl::pushModelMatrix();
	{
		gl::pushModelMatrix();
		gl::translate(getCurrentNotePos(0, 0));
		gl::scale(2.1, 2.1, 2.1);
		sampleDot->draw();
		gl::popModelMatrix();

		gl::pushModelMatrix();
		gl::translate(0.5, 0.5, 1.5);
		gl::scale(3.1, 3.1, 3.1);
		sampleDot->draw();
		gl::popModelMatrix();


		gl::ScopedLineWidth lineWidth(2.0f);
		gl::color(1, 0, 0);
		gl::drawLine(vec3(0, 0, 0), vec3(10, 0, 0));

		gl::color(0, 1, 0);
		gl::drawLine(vec3(0, 0, 0), vec3(0, 10, 0));

		gl::color(0, 0, 1);
		gl::drawLine(vec3(0, 0, 0), vec3(0, 0, 10));

		for (int i = 0; i < armPositionL.size(); i++) {
			gl::pushModelMatrix();
			gl::translate(armPositionL[i]);
			gl::scale(3.1, 3.1, 3.1);
			sampleDot->draw();
			gl::popModelMatrix();
		}

		for (int i = 0; i < armPositionR.size(); i++) {
			gl::pushModelMatrix();
			gl::translate(armPositionR[i]);
			gl::scale(3.1, 3.1, 3.1);
			sampleDot->draw();
			gl::popModelMatrix();

		}

		gl::pushModelMatrix();
		{
			drawBody();
			drawHead();
			drawArms();
		}
		gl::popModelMatrix();
		gl::pushModelMatrix();
		{
			gl::translate(1, 1, 1);
			gl::rotate(angleAxis(float(-M_PI / 4), vec3(0, 0, 1)));
			guitar.draw();
			drawFrets();
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
