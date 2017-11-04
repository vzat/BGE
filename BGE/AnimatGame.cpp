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
