#pragma once
#include "ECS/Components/ComponentBase.h"

class ComponentRespawn : public ComponentBase
{
	sf::Time mDefaultRespawnCounter;
	sf::Time mCurrentRespawnCounter;

public:
	ComponentRespawn():
		ComponentBase(EComponentType::Respawn)
	{}

	const sf::Time& GetDefaultRespawnCounter() const { return mDefaultRespawnCounter; }
	void SetDefaultRespawnCounter(const sf::Time& InTimeRate)
	{ 
		mDefaultRespawnCounter = InTimeRate; 
		mCurrentRespawnCounter = mDefaultRespawnCounter;
	}

	const sf::Time& GetCurrentRespawnCounter() const { return mCurrentRespawnCounter; }
	void SetCurrentRespawnCounter(const sf::Time& InTimeRate) { mCurrentRespawnCounter = InTimeRate; }
};