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

	dynamicsWorld->setGravity(btVector3(0, -9, 0));

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

	//CreateAnimat(glm::vec3(0, 10, 0), 10);
	animat = CreateZombie(glm::vec3(0, 0, 0), 5);

	if (!Game::Initialise()) {
		return false;
	}

	camera->transform->position = glm::vec3(0, 10, 20);

	return true;
}

void BGE::AnimatGame::Update(float timeDelta)
{
	//animat.arm1->rigidBody->applyTorque(timeDelta * 100 * btVector3(500.0f, 0.0f, 0.0f));
	//animat.arm2->rigidBody->applyTorque(timeDelta * 100 * btVector3(500.0f, 0.0f, 0.0f));

	btVector3 force = timeDelta * 5000 * btVector3(0.0f, 1.0f, -0.5f);
	animat.arm1->rigidBody->applyForce(force, btVector3(0.0f, 0.0f, -5 / 4));
	animat.arm2->rigidBody->applyForce(force, btVector3(0.0f, 0.0f, -5 / 4));
	
	//animat.body->rigidBody->applyTorque(timeDelta * 100 * btVector3(0.0f, 100.0f, 0.0f));

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

zombieRigid BGE::AnimatGame::CreateZombie(glm::vec3 position, float totalSize)
{
	// Body
	float bodyLength = totalSize;
	float bodyWidth = bodyLength / 2;
	float bodyHeight = bodyWidth / 2;
	shared_ptr<PhysicsController> body = physicsFactory->CreateBox(bodyWidth, bodyHeight, bodyLength, position, glm::quat());

	// Head
	//float headRadius = bodyWidth / 8;
	//glm::vec3 headOffset = glm::vec3(0, 0, bodyLength / 2 + headRadius + 0.1);
	//shared_ptr<PhysicsController> head = physicsFactory->CreateSphere(headRadius, position + headOffset, glm::quat());

	//btTransform bodyHeadT, headBodyT;
	//bodyHeadT.setIdentity();
	//bodyHeadT.setOrigin(btVector3(0, 0, headOffset.z));

	//headBodyT.setIdentity();
	//headBodyT.setOrigin(btVector3(0, 0, headRadius));

	//btFixedConstraint *headBody = new btFixedConstraint(*head->rigidBody, *body->rigidBody, headBodyT, bodyHeadT);
	//dynamicsWorld->addConstraint(headBody);


	// Arms
	float armRadius = bodyWidth / 8;
	float armLength = bodyLength / 2;

	// Left Arm
	glm::vec3 arm1Offset = glm::vec3(bodyWidth / 2 + armRadius, 0, bodyLength / 2 + armLength / 4);
	shared_ptr<PhysicsController> arm1 = physicsFactory->CreateBox(armRadius * 2, armRadius * 2, armLength, position + arm1Offset, glm::quat());

	btHingeConstraint *arm1Body = new btHingeConstraint(*arm1->rigidBody, *body->rigidBody, btVector3(-armRadius, 0, armLength / 4), btVector3(arm1Offset.x, arm1Offset.y, arm1Offset.z), btVector3(1, 0, 0), btVector3(1, 0, 0));
	arm1Body->setLimit(-4, 4);
	dynamicsWorld->addConstraint(arm1Body);

	// Right Arm
	glm::vec3 arm2Offset = glm::vec3(- bodyWidth / 2 - armRadius, 0, bodyLength / 2 + armLength / 4);
	shared_ptr<PhysicsController> arm2 = physicsFactory->CreateBox(armRadius * 2, armRadius * 2, armLength, position + arm2Offset, glm::quat());

	btHingeConstraint *arm2Body = new btHingeConstraint(*arm2->rigidBody, *body->rigidBody, btVector3(+armRadius, 0, armLength / 4), btVector3(arm2Offset.x, arm2Offset.y, arm2Offset.z), btVector3(1, 0, 0), btVector3(1, 0, 0));
	arm2Body->setLimit(-4, 4);
	dynamicsWorld->addConstraint(arm2Body);


	// Add Stuff to the back
	int noStuff = 1;
	float stuffRadius = bodyWidth / 6;
	glm::vec3 stuffOffset = glm::vec3(bodyWidth / 2, bodyHeight + stuffRadius * 2, 0);
	for (int i = 0; i < noStuff; i++) {
		shared_ptr<PhysicsController> stuff = physicsFactory->CreateBox(stuffRadius * 2, stuffRadius * 2, stuffRadius * 2, position + stuffOffset, glm::quat());

		btTransform stuffBodyT, bodyStuffT;
		bodyStuffT.setIdentity();
		bodyStuffT.setOrigin(btVector3(0.0f, bodyHeight / 2, -bodyLength / 2));

		stuffBodyT.setIdentity();
		stuffBodyT.setOrigin(btVector3(0, -stuffRadius, 0));

		btFixedConstraint *stuffBody = new btFixedConstraint(*stuff->rigidBody, *body->rigidBody, stuffBodyT, bodyStuffT);
		dynamicsWorld->addConstraint(stuffBody);
	}

	zombieRigid zombie;

	zombie.body = body;
	zombie.arm1 = arm1;
	zombie.arm2 = arm2;

	return zombie;
}

shared_ptr<PhysicsController> BGE::AnimatGame::CreateAnimat(glm::vec3 position, float totalSize)
{
	// Body
	float bodyLength = totalSize;
	float bodyWidth = bodyLength / 4;
	float bodyHeight = bodyWidth / 2;
	shared_ptr<PhysicsController> body = physicsFactory->CreateBox(bodyWidth, bodyHeight, bodyLength, position, glm::quat());

	// Front Legs
	float frontLegRadius = bodyHeight / 8;
	float frontLegSegmentLength = bodyWidth;
	
	float frontLegSegmentAngle1 = 45.0f;
	glm::vec3 frontLegSegmentOffset1 = glm::vec3(bodyWidth / 2, - bodyHeight / 2 - frontLegSegmentLength / 2, bodyLength / 2);
	glm::quat frontLegSegmentQuat1 = glm::angleAxis(frontLegSegmentAngle1, glm::vec3(0, 0, 1));
	shared_ptr<PhysicsController> frontLegSegmentTop = physicsFactory->CreateCylinder(frontLegRadius, frontLegSegmentLength, position + frontLegSegmentOffset1, frontLegSegmentQuat1);

	//// Body
	//float bodyLength = totalSize;
	//float bodyRadius = getPercentage(bodyLength, 10);
	//float bodyAngle = 90;
	//// Rotate body 90 degrees so it's sitting horizontally
	//glm::quat bodyAngleQuat = glm::angleAxis(bodyAngle, glm::vec3(1, 0, 0));
	//shared_ptr<PhysicsController> body = physicsFactory->CreateCylinder(bodyRadius, bodyLength, position, bodyAngleQuat);

	//
	//// Front Legs
	//std::vector<shared_ptr<PhysicsController>> frontLegs;

	//int noPairsFrontLegs = 3;
	//float frontLegLength = getPercentage(bodyLength, 33);
	//float frontLegRadius = getPercentage(frontLegLength, 10);
	//float frontLegAngle = bodyAngle - 90;
	//float frontLegDistance = getPercentage(bodyLength, 15);
	//glm::quat frontLegAngleQuat = glm::angleAxis(frontLegAngle, glm::vec3(1, 0, 0));
	//for (int i = 0; i < noPairsFrontLegs; i++)
	//{
	//	glm::vec3 offset1 = glm::vec3(bodyRadius - frontLegRadius, - frontLegLength / 2 - bodyRadius, bodyLength / 2 - frontLegRadius - i * frontLegDistance);
	//	shared_ptr<PhysicsController> leg1 = physicsFactory->CreateCylinder(frontLegRadius, frontLegLength, position + offset1, frontLegAngleQuat);
	//
	//	glm::vec3 offset2 = offset1 - glm::vec3(2 * bodyRadius - 2 * frontLegRadius, 0, 0);
	//	shared_ptr<PhysicsController> leg2 = physicsFactory->CreateCylinder(frontLegRadius, frontLegLength, position + offset2, frontLegAngleQuat);

	//	btTransform leg1T, leg2T, bodyLeg1T, bodyLeg2T;
	//	leg1T.setIdentity();
	//	leg1T.setRotation(toBtQuat(frontLegAngleQuat));
	//	leg1T.setOrigin(btVector3(0, - frontLegLength / 2, 0));

	//	bodyLeg1T.setIdentity();
	//	bodyLeg1T.setRotation(toBtQuat(bodyAngleQuat));
	//	bodyLeg1T.setOrigin(btVector3(bodyRadius, bodyLength / 2 - i * frontLegDistance, bodyRadius));

	//	btFixedConstraint *frontLeg1_Body = new btFixedConstraint(*body->rigidBody, *leg1->rigidBody, bodyLeg1T, leg1T);
	//	dynamicsWorld->addConstraint(frontLeg1_Body);


	//	leg2T.setIdentity();
	//	leg2T.setRotation(toBtQuat(frontLegAngleQuat));
	//	leg2T.setOrigin(btVector3(0, -frontLegLength / 2, 0));

	//	bodyLeg2T.setIdentity();
	//	bodyLeg2T.setRotation(toBtQuat(bodyAngleQuat));
	//	bodyLeg2T.setOrigin(btVector3(-bodyRadius, bodyLength / 2 - i * frontLegDistance, bodyRadius));

	//	btFixedConstraint *frontLeg2_Body = new btFixedConstraint(*body->rigidBody, *leg2->rigidBody, bodyLeg2T, leg2T);
	//	dynamicsWorld->addConstraint(frontLeg2_Body);

	//	frontLegs.push_back(leg1);
	//	frontLegs.push_back(leg2);
	//}

	return body;
}

float BGE::AnimatGame::getPercentage(float value, float percentage)
{
	return percentage / 100 * value;
}

btQuaternion BGE::AnimatGame::toBtQuat(glm::quat quat)
{
	return btQuaternion(quat[0], quat[1], quat[2], quat[3]);
}