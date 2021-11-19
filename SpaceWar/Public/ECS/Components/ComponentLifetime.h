#pragma once
#include "ECS/Components/ComponentBase.h"

class ComponentLifetime : public ComponentBase{

	sf::Time mDefaultLifeTime;
	sf::Time mCurrentLifeTime;

public:
	ComponentLifetime():
		ComponentBase(EComponentType::Lifetime)
	{}

	void SetDefaultLifeTime(const sf::Time InDefaultLifeTime) 
	{ 
		mDefaultLifeTime = InDefaultLifeTime; 
		mCurrentLifeTime = mDefaultLifeTime;
	}
	const sf::Time& GetDefaultLifeTime() const { return mDefaultLifeTime; }

	void SetCurrentLifeTime(const sf::Time InCurrentLifeTime) { mCurrentLifeTime = InCurrentLifeTime; }
	const sf::Time& GetCurrentLifeTime() const { return mCurrentLifeTime; }
};