#include "AnimatGame.h"

using namespace BGE;

AnimatGame::AnimatGame(void)
{
}

AnimatGame::~AnimatGame(void)
{
}

bool AnimatGame::Initialise()
{
	physicsFactory->CreateGroundPhysics();
	physicsFactory->CreateCameraPhysics();

	//dynamicsWorld->setGravity(btVector3(0, -9, 0));

	// Create Walls
	int noWalls = rand() % 3 + 1;
	int maxBlocks = 5;
	int minBlocks = 2;
	for (int i = 0; i < noWalls; i++)
	{
		int blockSize = rand() % 3 + 1;
		int blocksWidth = rand() % (maxBlocks - 1) + minBlocks;
		int blocksHeight = rand() % (maxBlocks - 1) + minBlocks;
		CreateWall(glm::vec3(rand() % 100, 0, rand() % 100), blockSize, blocksWidth, blocksHeight);
	}

	CreateAnimat(glm::vec3(0, 10, 0), 10);

	if (!Game::Initialise()) {
		return false;
	}

	camera->transform->position = glm::vec3(0, 10, 20);

	return true;
}

void BGE::AnimatGame::Update(float timeDelta)
{
	Game::Update(timeDelta);
}

void BGE::AnimatGame::Cleanup()
{
	Game::Cleanup();
}

std::vector<std::vector<shared_ptr<PhysicsController>>> BGE::AnimatGame::CreateWall(glm::vec3 position, int blockSize, int noWidth, int noHeight)
{
	// Store the boxes in a wall vector
	std::vector<std::vector<shared_ptr<PhysicsController>>> wall;
	for (int i = 0; i < noWidth; i++)
	{
		std::vector<shared_ptr<PhysicsController>> line;
		wall.push_back(line);
		for (int j = 0; j < noHeight; j++)
		{
			shared_ptr<PhysicsController> box = physicsFactory->CreateBox(blockSize, blockSize, blockSize, position + glm::vec3(i * blockSize, j * blockSize, 0), glm::quat());
			line.push_back(box);
		}
	}

	return wall;
}

void BGE::AnimatGame::CreateAnimat(glm::vec3 position, float totalSize)
{
	// Body
	float bodyLength = totalSize;
	float bodyRadius = getPercentage(bodyLength, 10);
	float bodyAngle = 90;
	// Rotate body 90 degrees so it's sitting horizontally
	glm::quat bodyAngleQuat = glm::angleAxis(bodyAngle, glm::vec3(1, 0, 0));
	shared_ptr<PhysicsController> body = physicsFactory->CreateCylinder(bodyRadius, bodyLength, position, bodyAngleQuat);

	
	// Front Legs
	std::vector<shared_ptr<PhysicsController>> frontLegs;

	int noPairsFrontLegs = 3;
	float frontLegLength = getPercentage(bodyLength, 33);
	float frontLegRadius = getPercentage(frontLegLength, 10);
	float frontLegAngle = bodyAngle - 90;
	float frontLegDistance = getPercentage(bodyLength, 15);
	glm::quat frontLegAngleQuat = glm::angleAxis(frontLegAngle, glm::vec3(1, 0, 0));
	for (int i = 0; i < noPairsFrontLegs; i++)
	{
		glm::vec3 offset1 = glm::vec3(bodyRadius - frontLegRadius, - frontLegLength / 2 - bodyRadius, bodyLength / 2 - frontLegRadius - i * frontLegDistance);
		shared_ptr<PhysicsController> leg1 = physicsFactory->CreateCylinder(frontLegRadius, frontLegLength, position + offset1, frontLegAngleQuat);
	
		glm::vec3 offset2 = offset1 - glm::vec3(2 * bodyRadius - 2 * frontLegRadius, 0, 0);
		shared_ptr<PhysicsController> leg2 = physicsFactory->CreateCylinder(frontLegRadius, frontLegLength, position + offset2, frontLegAngleQuat);

		btTransform leg1T, leg2T, bodyLeg1T, bodyLeg2T;
		leg1T.setIdentity();
		leg1T.setRotation(toBtQuat(frontLegAngleQuat));
		leg1T.setOrigin(btVector3(0, - frontLegLength / 2, 0));

		bodyLeg1T.setIdentity();
		bodyLeg1T.setRotation(toBtQuat(bodyAngleQuat));
		bodyLeg1T.setOrigin(btVector3(bodyRadius, bodyLength / 2 - i * frontLegDistance, bodyRadius));

		btFixedConstraint *frontLeg1_Body = new btFixedConstraint(*body->rigidBody, *leg1->rigidBody, bodyLeg1T, leg1T);
		dynamicsWorld->addConstraint(frontLeg1_Body);


		leg2T.setIdentity();
		leg2T.setRotation(toBtQuat(frontLegAngleQuat));
		leg2T.setOrigin(btVector3(0, -frontLegLength / 2, 0));

		bodyLeg2T.setIdentity();
		bodyLeg2T.setRotation(toBtQuat(bodyAngleQuat));
		bodyLeg2T.setOrigin(btVector3(-bodyRadius, bodyLength / 2 - i * frontLegDistance, bodyRadius));

		btFixedConstraint *frontLeg2_Body = new btFixedConstraint(*body->rigidBody, *leg2->rigidBody, bodyLeg2T, leg2T);
		dynamicsWorld->addConstraint(frontLeg2_Body);

		frontLegs.push_back(leg1);
		frontLegs.push_back(leg2);
	}
}

float BGE::AnimatGame::getPercentage(float value, float percentage)
{
	return percentage / 100 * value;
}

btQuaternion BGE::AnimatGame::toBtQuat(glm::quat quat)
{
	return btQuaternion(quat[0], quat[1], quat[2], quat[3]);
}