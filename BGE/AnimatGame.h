#pragma once
#ifndef ANIMAT_ANIMATGAME_H
#define ANIMAT_ANIMATGAME_H
#include "Game.h"
#include "PhysicsFactory.h"
#include "Box.h"
#include <vector>

namespace BGE
{
	class AnimatGame : public Game
	{
	private:

	public:
		AnimatGame(void);
		~AnimatGame(void);
		bool Initialise();
		void Update(float timeDelta);
		void Cleanup();
		std::vector<std::vector<shared_ptr<PhysicsController>>> CreateWall(glm::vec3 origin, int blockSize, int noWidth, int noHeight);
	};
}

#endif ANIMAT_ANIMATGAME_H