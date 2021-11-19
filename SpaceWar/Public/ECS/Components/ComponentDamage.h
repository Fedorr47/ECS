#pragma once
#include "ECS/Components/ComponentBase.h"

class ComponentDamage : public ComponentBase{
	int mDefaultDamageValue{0};
	int mCurrenttDamageValue{0};
public:
	ComponentDamage():
		ComponentBase(EComponentType::Damage)
	{}

	void SetDefaultDamageValue(const int InDamageValue) 
	{ 
		mDefaultDamageValue = InDamageValue; 
		mCurrenttDamageValue = mDefaultDamageValue;
	}
	const int GetDefaultDamageValue() const { return mDefaultDamageValue; }

	void SetCurrentDamageValue(const int InDamageValue) { mCurrenttDamageValue = InDamageValue; }
	const int GetCurrentDamageValue() const { return mCurrenttDamageValue; }
};