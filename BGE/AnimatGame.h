#pragma once
#ifndef ANIMAT_ANIMATGAME_H
#define ANIMAT_ANIMATGAME_H
#include "Game.h"
#include "PhysicsFactory.h"
#include "Box.h"
#include <vector>
#include <btBulletDynamicsCommon.h>

namespace BGE
{
	struct zombieHand {
		shared_ptr<PhysicsController> hand;
		std::vector<shared_ptr<PhysicsController>> fingers;
	};

	struct zombieArm {
		shared_ptr<PhysicsController> arm;
		btHingeConstraint *armJoint;
		zombieHand hand;
	};

	struct zombie {
		shared_ptr<PhysicsController> body;
		shared_ptr<PhysicsController> head;
		zombieArm arm1, arm2;
	};

	struct objSize {
		float width;
		float height;
		float depth;
	};

	class AnimatGame : public Game
	{
	private:

	public:
		AnimatGame(void);
		~AnimatGame(void);
		bool Initialise();
		void Update(float timeDelta);
		void Cleanup();

		std::vector<std::vector<shared_ptr<PhysicsController>>> CreateWall(glm::vec3 position, int blockSize, int noWidth, int noHeight);
		zombie CreateZombie(glm::vec3 position, float totalSize);
		void colourObject(shared_ptr<PhysicsController> object, glm::vec3 colour);
		void CreateHat(shared_ptr<PhysicsController> head, glm::vec3 headPosition, float headRadius, float radius, float height);
		zombieArm CreateArm(shared_ptr<PhysicsController> body, glm::vec3 bodyPosition, glm::vec3 armOffset, float armLength, float armRadius, bool right);
		zombieHand CreateHand(shared_ptr<PhysicsController> arm, glm::vec3 armPosition, glm::vec3 handOffset, float handWidth, float handHeight);
		void CreateEye(shared_ptr<PhysicsController> head, glm::vec3 headPosition, glm::vec3 eyeOffset, float eyeRadius);

		zombie animat;
	};
}

#endif ANIMAT_ANIMATGAME_H