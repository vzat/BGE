#include "ZombieArm.h"

using namespace BGE;

ZombieArm::ZombieArm(std::shared_ptr<PhysicsFactory> physicsFactory, btDiscreteDynamicsWorld *dynamicsWorld) : GameComponent(true)
{
	this->physicsFactory = physicsFactory;
	this->dynamicsWorld = dynamicsWorld;
}

ZombieArm::ZombieArm(void)
{
}

ZombieArm::~ZombieArm(void)
{
}

bool ZombieArm::Initialise()
{
	return true;
}

void ZombieArm::Update(float timeDelta)
{
	GameComponent::Update(timeDelta);
}

void BGE::ZombieArm::CreateArm(shared_ptr<PhysicsController> body, glm::vec3 bodyPosition, glm::vec3 armOffset, float armLength, float armRadius, bool right)
{
	arm = physicsFactory->CreateBox(armRadius * 2, armRadius * 2, armLength, bodyPosition + armOffset, glm::quat());

	float armSide = 1;
	if (!right) {
		armSide = -1;
	}

	armJoint = new btHingeConstraint(*arm->rigidBody, *body->rigidBody, btVector3(armSide * armRadius, 0, armLength / 2), btVector3(armOffset.x, armOffset.y, armOffset.z), btVector3(1, 0, 0), btVector3(1, 0, 0));
	if (!right) {
		armJoint->enableAngularMotor(true, 12, 17);
	}
	else {
		armJoint->enableAngularMotor(true, 12, 19);
	}

	dynamicsWorld->addConstraint(armJoint);

	float handWidth = armRadius * 3;
	float handHeight = armRadius;
	glm::vec3 handOffset = glm::vec3(0, 0, -armLength / 2 - handWidth / 2);

	hand = make_shared<ZombieHand>(physicsFactory, dynamicsWorld);
	hand->CreateHand(arm, bodyPosition + armOffset, handOffset, handWidth, handHeight);
}