#pragma once
#include "ECS/Components/ComponentBase.h"

class ComponentCollidable : public ComponentBase 
{
	sf::FloatRect mAABB;
	bool mCollidingOnX;
	bool mCollidingOnY;

public:

	ComponentCollidable() : ComponentBase(EComponentType::Collidable),
		mCollidingOnX(false),
		mCollidingOnY(false)
	{}

	void SetCollidable(const sf::FloatRect& InRect) { mAABB = InRect; }
	sf::FloatRect GetCollidable() const { return mAABB; }
	const sf::FloatRect& GetCollidable() { return mAABB; }

	bool IsCollidingOnX() { return mCollidingOnX; }
	bool IsCollidingOnY() { return mCollidingOnY; }

	void CollideOnX() { mCollidingOnX = true; }
	void CollideOnY() { mCollidingOnY = true; }

	void ResetCollisionFlags() {
		mCollidingOnX = false;
		mCollidingOnY = false;
	}
};