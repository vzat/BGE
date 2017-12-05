#include "AnimatGame.h"
//#include <glm/gtx/rotate_vector.hpp>

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
		int blockSize = rand() % 2 + 1;
		int blocksWidth = rand() % (maxBlocks - 1) + minBlocks;
		int blocksHeight = rand() % (maxBlocks - 1) + minBlocks;
		CreateWall(glm::vec3(rand() % 100, 0, rand() % 100), blockSize, blocksWidth, blocksHeight);
	}

	animat = CreateZombie(glm::vec3(0, 0, 0), 5);
	CreateZombie(glm::vec3(-10, 0, 10), 5);
	CreateZombie(glm::vec3(10, 0, 10), 5);

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

zombie BGE::AnimatGame::CreateZombie(glm::vec3 position, float totalSize)
{
	// Body
	float bodyLength = totalSize;
	float bodyWidth = bodyLength / 2;
	float bodyHeight = bodyWidth / 3;
	shared_ptr<PhysicsController> body = physicsFactory->CreateBox(bodyWidth, bodyHeight, bodyLength, position, glm::quat());
	colourObject(body, glm::vec3(rand() % 100, rand() % 100, rand() % 100));

	// Head
	float headRadius = bodyWidth / 5;
	glm::vec3 headOffset = glm::vec3(0, bodyHeight / 2 + headRadius, bodyLength / 2);
	shared_ptr<PhysicsController> head = physicsFactory->CreateSphere(headRadius, position + headOffset, glm::quat());
	colourObject(head, glm::vec3(55.0f, 15.0f, 90.0f));

	btTransform bodyHeadT, headBodyT;
	bodyHeadT.setIdentity();
	bodyHeadT.setOrigin(btVector3(headOffset.x, headOffset.y, headOffset.z));

	headBodyT.setIdentity();
	headBodyT.setOrigin(btVector3(0, 0, headRadius));

	btFixedConstraint *headBody = new btFixedConstraint(*head->rigidBody, *body->rigidBody, headBodyT, bodyHeadT);
	dynamicsWorld->addConstraint(headBody);

	// Eyes
	CreateEye(head, position + headOffset, glm::vec3(- headRadius / 2, headRadius / 2, headRadius), headRadius / 4);
	CreateEye(head, position + headOffset, glm::vec3(+ headRadius / 2, headRadius / 2, headRadius), headRadius / 4);

	// Hat
	CreateHat(head, position + headOffset, headRadius, headRadius, headRadius);

	//// Arms
	float armRadius = bodyWidth / 15;
	float armLength = bodyLength / 2;
	CreateArm(body, position, glm::vec3(+bodyWidth / 2 + armRadius, 0, bodyLength / 2 - armLength / 8), armLength, armRadius, false);
	CreateArm(body, position, glm::vec3(-bodyWidth / 2 - armRadius, 0, bodyLength / 2 - armLength / 8), armLength, armRadius, true);

	zombie zombie;
	zombie.body = body;
	zombie.head = head;

	return zombie;
}


void BGE::AnimatGame::CreateHat(shared_ptr<PhysicsController> head, glm::vec3 headPosition, float headRadius, float radius, float height) {
	glm::quat cylinderQuat = glm::angleAxis(90.0f, glm::vec3(1, 0, 0));

	// Base Hat
	float baseRadius = radius;
	float baseHeight = height / 3;
	glm::vec3 baseOffset = glm::vec3(0, headRadius, 0);
	shared_ptr<PhysicsController> hatBase = physicsFactory->CreateCylinder(baseRadius, baseHeight, headPosition + baseOffset, cylinderQuat);

	colourObject(hatBase, glm::vec3(255.0f, 255.0f, 255.0f));

	btTransform baseHeadT, headBaseT;
	baseHeadT.setIdentity();
	baseHeadT.setOrigin(btVector3(baseOffset.x, baseOffset.y, baseOffset.z));

	headBaseT.setIdentity();
	headBaseT.setOrigin(btVector3(0, -baseHeight / 2, 0));

	btFixedConstraint *baseHead = new btFixedConstraint(*hatBase->rigidBody, *head->rigidBody, headBaseT, baseHeadT);
	dynamicsWorld->addConstraint(baseHead);


	// Middle Hat
	float middleRadius = radius * 3 / 4;
	float middleHeight = height * 2 / 3;
	glm::vec3 middleOffset = glm::vec3(0, baseHeight / 2, 0);
	shared_ptr<PhysicsController> hatMiddle = physicsFactory->CreateCylinder(middleRadius, middleHeight, headPosition + baseOffset + middleOffset, cylinderQuat);

	colourObject(hatMiddle, glm::vec3(100.0f, 0.0f, 0.0f));

	btTransform middleBaseT, baseMiddleT;
	middleBaseT.setIdentity();
	middleBaseT.setOrigin(btVector3(middleOffset.x, middleOffset.y, middleOffset.z));

	baseMiddleT.setIdentity();
	baseMiddleT.setOrigin(btVector3(0, -middleHeight / 2, 0));

	btFixedConstraint *middleBase = new btFixedConstraint(*hatMiddle->rigidBody, *hatBase->rigidBody, baseMiddleT, middleBaseT);
	dynamicsWorld->addConstraint(middleBase);


	// Hat Top
	float topRadius = radius / 5;
	float topLength = height * 4 / 3;

	glm::vec3 topOffset = glm::vec3(0, middleHeight / 2 + topRadius, 0);
	shared_ptr<PhysicsController> hatTop = physicsFactory->CreateCylinder(topRadius, topLength, headPosition + baseOffset + middleOffset + topOffset, cylinderQuat);

	colourObject(hatTop, glm::vec3(100.0f, 0.0f, 0.0f));

	btTransform topMiddleT, headTopT;
	topMiddleT.setIdentity();
	topMiddleT.setOrigin(btVector3(topOffset.x, topOffset.y, topOffset.z));

	headTopT.setIdentity();
	headTopT.setOrigin(btVector3(0, topLength / 2, 0));

	btGeneric6DofConstraint *topMiddle = new btGeneric6DofConstraint(*hatTop->rigidBody, *hatMiddle->rigidBody, headTopT, topMiddleT, true);
	dynamicsWorld->addConstraint(topMiddle);

	// Hat Ball
	float ballRadius = topRadius * 2;
	glm::vec3 ballOffset = glm::vec3(0, topLength / 2, 0);
	shared_ptr<PhysicsController> hatBall = physicsFactory->CreateSphere(ballRadius, headPosition + baseOffset + middleOffset + topOffset + ballOffset, glm::quat());

	colourObject(hatBall, glm::vec3(255.0f, 255.0f, 255.0f));

	btTransform topHatBallT, ballTopHatT;
	topHatBallT.setIdentity();
	topHatBallT.setOrigin(btVector3(ballOffset.x, ballOffset.y, ballOffset.z));

	ballTopHatT.setIdentity();
	ballTopHatT.setOrigin(btVector3(0, -ballRadius, 0));

	btFixedConstraint *topHatBall = new btFixedConstraint(*hatTop->rigidBody, *hatBall->rigidBody, ballTopHatT, topHatBallT);
	dynamicsWorld->addConstraint(topHatBall);
}

void BGE::AnimatGame::CreateArm(shared_ptr<PhysicsController> body, glm::vec3 bodyPosition, glm::vec3 armOffset, float armLength, float armRadius, bool right = true) {
	shared_ptr<PhysicsController> arm = physicsFactory->CreateBox(armRadius * 2, armRadius * 2, armLength, bodyPosition + armOffset, glm::quat());
	colourObject(arm, glm::vec3(55.0f, 15.0f, 90.0f));

	float armSide = 1;
	if (!right) {
		armSide = -1;
	}

	btHingeConstraint *armBody = new btHingeConstraint(*arm->rigidBody, *body->rigidBody, btVector3(armSide * armRadius, 0, armLength / 2), btVector3(armOffset.x, armOffset.y, armOffset.z), btVector3(1, 0, 0), btVector3(1, 0, 0));
	if (!right) {
		armBody->enableAngularMotor(true, 12, 17);
	}
	else {
		armBody->enableAngularMotor(true, 12, 19);
	}
	
	dynamicsWorld->addConstraint(armBody);

	float handWidth = armRadius * 3;
	float handHeight = armRadius;
	glm::vec3 handOffset = glm::vec3(0, 0, -armLength / 2 - handWidth / 2);

	CreateHand(arm, bodyPosition + armOffset, handOffset, handWidth, handHeight);
}

void BGE::AnimatGame::CreateHand(shared_ptr<PhysicsController> arm, glm::vec3 armPosition, glm::vec3 handOffset, float handWidth, float handHeight) {
	shared_ptr<PhysicsController> hand = physicsFactory->CreateBox(handWidth, handHeight, handWidth, armPosition + handOffset, glm::quat());
	colourObject(hand, glm::vec3(55.0f, 15.0f, 90.0f));

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

		btPoint2PointConstraint *fingerHand = new btPoint2PointConstraint(*finger->rigidBody, *hand->rigidBody, btVector3(0, 0, fingerLength / 2), btVector3(armFingerOffset.x, armFingerOffset.y, armFingerOffset.z));
		dynamicsWorld->addConstraint(fingerHand);
	}
}

void  BGE::AnimatGame::CreateEye(shared_ptr<PhysicsController> head, glm::vec3 headPosition, glm::vec3 eyeOffset, float eyeRadius) {
	shared_ptr<PhysicsController> eye = physicsFactory->CreateSphere(eyeRadius, headPosition + eyeOffset, glm::quat());
	colourObject(eye, glm::vec3(255.0f, 255.0f, 255.0f));

	btTransform headEyeT, eyeHeadT;
	headEyeT.setIdentity();
	headEyeT.setOrigin(btVector3(eyeOffset.x, eyeOffset.y, eyeOffset.z));

	eyeHeadT.setIdentity();
	eyeHeadT.setOrigin(btVector3(0, 0, 0));

	btFixedConstraint *eyeHead = new btFixedConstraint(*eye->rigidBody, *head->rigidBody, eyeHeadT, headEyeT);
	dynamicsWorld->addConstraint(eyeHead);
}

void BGE::AnimatGame::colourObject(shared_ptr<PhysicsController> object, glm::vec3 colour) {
	((shared_ptr<GameComponent>) object)->transform->diffuse = colour / 255.0f;
	((shared_ptr<GameComponent>) object)->transform->specular = colour / 255.0f;
	((shared_ptr<GameComponent>) object)->transform->ambient = colour / 255.0f;
}