#pragma once
#ifndef ANIMAT_ANIMATGAME_H
#define ANIMAT_ANIMATGAME_H
#include "Game.h"

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
	};
}

#endif ANIMAT_ANIMATGAME_H