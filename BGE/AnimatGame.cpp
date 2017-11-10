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

	CreateAnimat(glm::vec3(0, 0, 0));

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

void BGE::AnimatGame::CreateAnimat(glm::vec3 position)
{
	int bodyRadius = 2;
	int bodyLength = 10;
	float theta = 90;
	glm::quat angleQuat = glm::angleAxis(theta, glm::vec3(1, 0, 0));
	shared_ptr<PhysicsController> body = physicsFactory->CreateCylinder(bodyRadius, bodyLength, position, angleQuat);
}