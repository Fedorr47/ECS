#pragma once
#include "ECS/Components/ComponentBase.h"

class ComponentHealth : public ComponentBase{
	
	int mCurrentHealthPoints{ 0 };
	int mMaxHealthPoints{ 0 };

public:
	ComponentHealth():
		ComponentBase(EComponentType::Health)
	{}

	void SetMaxHealth(const unsigned InHealthPoints) {
		mMaxHealthPoints = InHealthPoints;
		mCurrentHealthPoints = mMaxHealthPoints;
	}
	const int GetMaxHealth() const { return mMaxHealthPoints; }

	void SetHealth(const int InHealthPoints) { mCurrentHealthPoints = InHealthPoints; }
	const int GetHealth() const { return mCurrentHealthPoints; }
};