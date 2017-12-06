#pragma once
#ifndef ANIMAT_ZOMBIEARM_H
#define ANIMAT_ZOMBIEARM_H
#include "GameComponent.h"
#include "PhysicsFactory.h"
#include <vector>
#include <btBulletDynamicsCommon.h>
#include "Game.h"
#include "ZombieHand.h"

namespace BGE
{
	class ZombieArm : public GameComponent
	{
	public:
		ZombieArm(std::shared_ptr<PhysicsFactory> physicsFactory, btDiscreteDynamicsWorld *dynamicsWorld);
		ZombieArm();
		~ZombieArm(void);
		bool Initialise();
		void Update(float timeDelta);
		void CreateArm(shared_ptr<PhysicsController> body, glm::vec3 bodyPosition, glm::vec3 armOffset, float armLength, float armRadius, bool right);

		std::shared_ptr<PhysicsFactory> physicsFactory;
		btDiscreteDynamicsWorld * dynamicsWorld;

		shared_ptr<PhysicsController> arm;
		btHingeConstraint *armJoint;
		shared_ptr<ZombieHand> hand;
	};
}

#endif ANIMAT_ZOMBIEARM_H