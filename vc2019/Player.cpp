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


	auto forearm = geom::Capsule().subdivisionsAxis(10)
		.subdivisionsHeight(10).length(1.0).radius(0.25);

	mArm = gl::Batch::create(forearm, shader);

	lHand = Hand(false, fingerAngles);
	rHand = Hand(true, fingerAngles);
	armRotations.resize(6, glm::vec3(M_PI / 6, 0, 0));
	armTranslations.resize(7);
	armTranslations = { vec3(-1.1, 0, 0), vec3(0, 1.333, 0),
						vec3(0, 0.9, 0), vec3(1.1, 0, 0),
						vec3(0, 1.333 ,0), vec3(0, 0.9, 0) };
}

void Player::update() {
}

vec3 Player::getCurrentNotePos() {
	return fretPositions[0][0];
	//return notes.front();
}

void Player::defineJoints() {
	lHandJoints.reserve(fingerLengths.size());
	rHandJoints.reserve(fingerLengths.size());
	lArmJoints.reserve(3);
	rArmJoints.reserve(3);

	vec3 position(0.0f); // Starting position
	vec3 angles(0.0f);   // Starting angles

	//Bicep, forearm and palm/wrist
	for (int i = 0; i < 3; i++) {
		Joint joint;
		joint.position = i == 0 ? vec3(1.1, 0, 0) : position;
		joint.length = i == 2 ? 0.8 : 1.3;
		joint.angles = angles;


		lHandJoints.push_back(joint);
		joint.position = -joint.position;
		rHandJoints.push_back(joint);
	}

	for (int i = 0; i < lHandJoints.size() - 1; i++) {
		lHandJoints[i].next = &lHandJoints[i + 1];
		lHandJoints[i + 1].prev = &lHandJoints[i];

		rHandJoints[i].next = &rHandJoints[i + 1];
		rHandJoints[i + 1].prev = &rHandJoints[i];
	}

	//Fingers

	for (float length : fingerLengths) {
		// Create a new joint
		Joint joint;
		joint.position = position;
		joint.length = length;
		joint.angles = angles;

		// Add the joint to the vector
		lHandJoints.push_back(joint);
		rHandJoints.push_back(joint);

		// Update position for the next joint (example: move along x-axis)
		position.x += length; // Update x-coordinate
	}


}

void Player::getJointPositions(Joint* curJoint, vec3 totalAngle, std::vector<vec3>* positions) {
	Joint* parent = curJoint->prev;
	vec3 curAngle = curJoint->angles + totalAngle;

	// Calculate position using trigonometric functions
	float curPositionX = parent->position.x + curJoint->length * std::cos(curAngle[0]); // X-coordinate
	float curPositionY = parent->position.y + curJoint->length * std::sin(curAngle[1]); // Y-coordinate
	float curPositionZ = parent->position.z + curJoint->length * std::tan(curAngle[2]); // Z-coordinate


	if (curJoint->next != nullptr) {
		getJointPositions(curJoint->next, curAngle, positions);
	}
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
	for (int i = 0; i <= 3; i += 3) {
		gl::pushModelMatrix();
		{
			gl::translate(armTranslations[0 + i]);
			Helpers::rotateFromBase(i == 0 ? armRootRotation : -armRootRotation,
				vec3(0, 0, 1), vec3(i == 0 ? 0.666 : -0.666, 0, 0)); // T - pose rotation from root
			allRotations(armRotations[0 + i], vec3(0, 0.666, 0));
			mArm->draw(); //Shoulder
			gl::pushModelMatrix();
			{
				gl::translate(armTranslations[1 + i]);
				allRotations(armRotations[1 + i], vec3(0, 0.666, 0));
				mArm->draw(); // Forearm
				gl::pushModelMatrix();
				{
					gl::translate(armTranslations[2 + i]);
					allRotations(armRotations[2 + i], vec3(0, 0.4, 0));
					drawHands(i == 0 ? true : false);
				}
				gl::popModelMatrix();
			}
			gl::popModelMatrix();
		}
		gl::popModelMatrix();
	}
}

void Player::allRotations(vec3 thetas, vec3 distance) {
	Helpers::rotateFromBase(float(thetas.x), vec3(1, 0, 0), distance);
	gl::rotate(angleAxis(float(thetas.y), vec3(0, 1, 0)));
	Helpers::rotateFromBase(float(thetas.z), vec3(0, 0, 1), distance);
}

std::vector<glm::vec3> Player::fabrik(std::vector<glm::vec3>& joint_positions, const glm::vec3& target_position, const std::vector<float>& distances, float tolerance) {
	int n = joint_positions.size();
	float dist = glm::length(joint_positions[0] - target_position);

	std::vector<glm::vec3> updated_joint_angles;
	updated_joint_angles.reserve(n);

	if (dist > std::accumulate(distances.begin(), distances.end(), 0.0f) + tolerance) {
		for (int i = 0; i < n - 1; i++) {
			float ri = glm::length(target_position - joint_positions[i]);
			float lambdai = distances[i] / ri;
			// Calculate the rotation axis and angle
			glm::vec3 axis = glm::normalize(glm::cross(joint_positions[i + 1] - joint_positions[i], target_position - joint_positions[i]));
			float angle = acos(glm::dot(glm::normalize(joint_positions[i + 1] - joint_positions[i]), glm::normalize(target_position - joint_positions[i])));
			// Apply rotation to the next joint
			glm::quat rotation = glm::angleAxis(angle, axis);
			joint_positions[i + 1] = joint_positions[i] + rotation * (joint_positions[i + 1] - joint_positions[i]) * lambdai;
			// Calculate and store the updated joint angles
			updated_joint_angles.push_back(rotation * glm::vec3(1.0f, 0.0f, 0.0f));
		}
	}
	else {
		glm::vec3 b = joint_positions[0];
		float difA = glm::length(joint_positions.back() - target_position);
		while (difA > tolerance) {
			// Stage 1: Forward Reaching
			joint_positions.back() = target_position;
			for (int i = n - 1; i > 0; i--) {
				float ri = glm::length(joint_positions[i + 1] - joint_positions[i]);
				float lambdai = distances[i] / ri;
				// Calculate the rotation axis and angle
				glm::vec3 axis = glm::normalize(glm::cross(joint_positions[i] - joint_positions[i + 1], joint_positions[i - 1] - joint_positions[i]));
				float angle = acos(glm::dot(glm::normalize(joint_positions[i] - joint_positions[i + 1]), glm::normalize(joint_positions[i - 1] - joint_positions[i])));
				// Apply rotation to the current joint
				glm::quat rotation = glm::angleAxis(angle, axis);
				joint_positions[i] = joint_positions[i + 1] + rotation * (joint_positions[i] - joint_positions[i + 1]) * lambdai;
				// Calculate and store the updated joint angles
				updated_joint_angles.push_back(rotation * glm::vec3(1.0f, 0.0f, 0.0f));
			}

			// Stage 2: Backward Reaching
			joint_positions[0] = b;
			for (int i = 0; i < n - 1; i++) {
				float ri = glm::length(joint_positions[i + 1] - joint_positions[i]);
				float lambdai = distances[i] / ri;
				// Calculate the rotation axis and angle
				glm::vec3 axis = glm::normalize(glm::cross(joint_positions[i] - joint_positions[i + 1], joint_positions[i + 2] - joint_positions[i + 1]));
				float angle = acos(glm::dot(glm::normalize(joint_positions[i] - joint_positions[i + 1]), glm::normalize(joint_positions[i + 2] - joint_positions[i + 1])));
				// Apply rotation to the next joint
				glm::quat rotation = glm::angleAxis(angle, axis);
				joint_positions[i + 1] = joint_positions[i] + rotation * (joint_positions[i + 1] - joint_positions[i]) * lambdai;
				// Calculate and store the updated joint angles
				updated_joint_angles.push_back(rotation * glm::vec3(1.0f, 0.0f, 0.0f));
			}

			difA = glm::length(joint_positions.back() - target_position);
		}
	}
	return updated_joint_angles;
}


//std::vector<glm::vec3> Player::fabrik(std::vector<glm::vec3>& joint_positions, const glm::vec3& target_position, const std::vector<float>& distances, float tolerance) {
//	int n = joint_positions.size();
//	float dist = glm::length(joint_positions[0] - target_position);
//
//	if (dist > std::accumulate(distances.begin(), distances.end(), 0.0f) + tolerance) {
//		for (int i = 0; i < n - 1; i++) {
//			float ri = glm::length(target_position - joint_positions[i]);
//			float lambdai = distances[i] / ri;
//			joint_positions[i + 1] = (1.0f - lambdai) * joint_positions[i] + lambdai * target_position;
//		}
//	}
//	else {
//		glm::vec3 b = joint_positions[0];
//		float difA = glm::length(joint_positions.back() - target_position);
//		while (difA > tolerance) {
//			// Stage 1: Forward Reaching
//			joint_positions.back() = target_position;
//			for (int i = n - 1; i > 0; i--) {
//				float ri = glm::length(joint_positions[i + 1] - joint_positions[i]);
//				float lambdai = distances[i] / ri;
//				joint_positions[i] = (1.0f - lambdai) * joint_positions[i + 1] + lambdai * joint_positions[i];
//			}
//
//			// Stage 2: Backward Reaching
//			joint_positions[0] = b;
//			for (int i = 0; i < n - 1; i++) {
//				float ri = glm::length(joint_positions[i + 1] - joint_positions[i]);
//				float lambdai = distances[i] / ri;
//				joint_positions[i + 1] = (1.0f - lambdai) * joint_positions[i] + lambdai * joint_positions[i + 1];
//			}
//
//			difA = glm::length(joint_positions.back() - target_position);
//		}
//	}
//	return joint_positions;
//}

void Player::draw() {
	//CI_LOG_V("MyApp draw() function");

	gl::pushModelMatrix();
	{
		//Rotate guitar, keep in world co-ords
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
							CI_LOG_V("String: " << i + 1 << ", Translation:" << currentTranslation);
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
		gl::popModelMatrix();
	}
	gl::popModelMatrix();
}

glm::vec3 Player::fretToWorldCoordinates(const glm::vec3& fretCoordinates)
{
	glm::mat4 inverseRotation = glm::rotate(glm::mat4(1.0f), float(-M_PI / 4), glm::vec3(0, 0, 1));
	glm::vec4 worldPosition = inverseRotation * glm::vec4(fretCoordinates, 1.0);

	return glm::vec3(worldPosition);
}

void Player::popN(int n) {
	for (int i = 0; i < n; i++) {
		gl::popModelMatrix();
	}
}
