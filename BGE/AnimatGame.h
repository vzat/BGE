#pragma once
#ifndef ANIMAT_ANIMATGAME_H
#define ANIMAT_ANIMATGAME_H
#include "Game.h"
#include "PhysicsFactory.h"
#include "Box.h"
#include <vector>
#include <btBulletDynamicsCommon.h>
//#include <glm/gtx/rotate_vector.hpp>

namespace BGE
{
	struct zombieRigid {
		shared_ptr<PhysicsController> body;
		shared_ptr<PhysicsController> arm1;
		shared_ptr<PhysicsController> arm2;
	};

	struct sharkRigid {
		shared_ptr<PhysicsController> body;
		shared_ptr<PhysicsController> tail;
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
		shared_ptr<PhysicsController> CreateAnimat(glm::vec3 position, float totalSize);
		zombieRigid CreateZombie(glm::vec3 position, float totalSize);
		sharkRigid CreateShark(glm::vec3 position, float totalSize);
		void colourObject(shared_ptr<PhysicsController> object, glm::vec3 colour);

		float getPercentage(float value, float percentage);

		btQuaternion toBtQuat(glm::quat quat);
		zombieRigid animat;
		sharkRigid shark;

		bool move;
		float arm1Scale, arm2Scale, bodyScale;
		float timeAcc;
		float tailDirection;
		float sharkDirection;
	};
}

#endif ANIMAT_ANIMATGAME_H