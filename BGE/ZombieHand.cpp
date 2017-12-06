#include "ZombieHand.h"

using namespace BGE;

ZombieHand::ZombieHand(std::shared_ptr<PhysicsFactory> physicsFactory, btDiscreteDynamicsWorld *dynamicsWorld) : GameComponent(true)
{
	this->physicsFactory = physicsFactory;
	this->dynamicsWorld = dynamicsWorld;
}

ZombieHand::ZombieHand(void)
{
}

ZombieHand::~ZombieHand(void)
{
}

bool ZombieHand::Initialise()
{
	return true;
}

void ZombieHand::Update(float timeDelta)
{
	GameComponent::Update(timeDelta);
}

void BGE::ZombieHand::CreateHand(shared_ptr<PhysicsController> arm, glm::vec3 armPosition, glm::vec3 handOffset, float handWidth, float handHeight)
{
	hand = physicsFactory->CreateBox(handWidth, handHeight, handWidth, armPosition + handOffset, glm::quat());

	btHingeConstraint *handArm = new btHingeConstraint(*hand->rigidBody, *arm->rigidBody, btVector3(0, 0, handWidth / 2), btVector3(handOffset.x, handOffset.y, handOffset.z), btVector3(1, 0, 0), btVector3(1, 0, 0));
	handArm->setLimit(-3.14 / 2, 3.14 / 2);
	dynamicsWorld->addConstraint(handArm);

	float fingerRadius = handHeight / 4;
	float fingerLength = handWidth;
	float noFingers = 3;
	float fingerDistance = handWidth / noFingers;

	for (int i = -noFingers / 2; i < noFingers / 2; i++) {
		glm::vec3 armFingerOffset = glm::vec3(i * fingerDistance, 0, -handWidth / 2);
		shared_ptr<PhysicsController> finger = physicsFactory->CreateBox(fingerRadius * 2, fingerRadius * 2, fingerLength, armPosition + handOffset + armFingerOffset, glm::quat());
		fingers.push_back(finger);

		btPoint2PointConstraint *fingerHand = new btPoint2PointConstraint(*finger->rigidBody, *hand->rigidBody, btVector3(0, 0, fingerLength / 2), btVector3(armFingerOffset.x, armFingerOffset.y, armFingerOffset.z));
		dynamicsWorld->addConstraint(fingerHand);
	}
}