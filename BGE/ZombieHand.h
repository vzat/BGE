#pragma once
#ifndef ANIMAT_ZOMBIEHAND_H
#define ANIMAT_ZOMBIEHAND_H
#include "GameComponent.h"
#include "PhysicsFactory.h"
#include <vector>
#include <btBulletDynamicsCommon.h>
#include "Game.h"

namespace BGE
{
	class ZombieHand : public GameComponent
	{
	public:
		ZombieHand(std::shared_ptr<PhysicsFactory> physicsFactory, btDiscreteDynamicsWorld *dynamicsWorld);
		ZombieHand();
		~ZombieHand(void);
		bool Initialise();
		void Update(float timeDelta);
		void CreateHand(shared_ptr<PhysicsController> arm, glm::vec3 armPosition, glm::vec3 handOffset, float handWidth, float handHeight);

		std::shared_ptr<PhysicsFactory> physicsFactory;
		btDiscreteDynamicsWorld * dynamicsWorld;

		shared_ptr<PhysicsController> hand;
		std::vector<shared_ptr<PhysicsController>> fingers;
	};
}

#endif ANIMAT_ZOMBIEHAND_H