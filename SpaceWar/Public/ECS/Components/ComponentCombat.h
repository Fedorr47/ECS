#pragma once

#include "ECS/Components/ComponentBase.h"

class ComponentCombat : public ComponentBase {
	
	int mMaxHealthPoints;
	int mCurrentHealthPoints;
	ETag mTag;
	ECombatEntityType mEntityType;
	int mDamageValue;
	sf::Time mDefaultRespawnCoolDown;
	sf::Time mCurrentRespawnCoolDown;
	sf::Time mDefaultLifeTime;
	sf::Time mCurrentLifeTime;
	
public:
	ComponentCombat() :
		ComponentBase(EComponentType::Combat),
		mMaxHealthPoints(0),
		mCurrentHealthPoints(0),
		mTag(ETag::MAX),
		mEntityType(ECombatEntityType::MAX),
		mDamageValue(0)
	{
		SetDefaultRespawnCoolDown(sf::seconds(0.0f));
		SetDefaultLifeTime(sf::seconds(0.0f));
	}
	~ComponentCombat() {}

	void SetMaxHealthPoints(int InHealthPoints) 
	{ 
		mMaxHealthPoints = InHealthPoints;
		mCurrentHealthPoints = mMaxHealthPoints;
	}
	int GetMaxHealthPoints() const { return mMaxHealthPoints; }

	void SetCurrentHealthPoints(unsigned int InHealthPoints) 
	{ 
		if (InHealthPoints < 0)
			InHealthPoints = 0;
		if (InHealthPoints > mMaxHealthPoints)
			InHealthPoints = mMaxHealthPoints;
		mCurrentHealthPoints = InHealthPoints; 
	}
	unsigned int GetCurrentHealthPoints() const { return mCurrentHealthPoints; }

	void SetTag(ETag InTag) { mTag = InTag; }
	ETag GetTag() const { return mTag; }

	void SetEntityType(const ECombatEntityType InEntityType) { mEntityType = InEntityType; }
	const ECombatEntityType& GetEntityType() const { return mEntityType; }

	void SetDefaultRespawnCoolDown(const sf::Time& InCoolDown) 
	{ 
		mDefaultRespawnCoolDown = InCoolDown; 
		mCurrentRespawnCoolDown = mDefaultRespawnCoolDown;
	}

	sf::Time GetDefaultRespawnCoolDown() const { return mDefaultRespawnCoolDown; }
	void SetCurrentRespawnCoolDown(const sf::Time& InCoolDown) { mCurrentRespawnCoolDown = InCoolDown; }
	sf::Time GetCurrentRespawnCoolDown() const { return mCurrentRespawnCoolDown; }

	void SetDefaultLifeTime(const sf::Time& InLifetime)
	{
		mDefaultLifeTime = InLifetime;
		mCurrentLifeTime = mDefaultLifeTime;
	}
	sf::Time GetDefaultLifeTime() const { return mDefaultLifeTime; }
	void SetCurrentLifeTime(const sf::Time& InLifetime) { mCurrentLifeTime = InLifetime; }
	sf::Time GetCurrentLifeTime() const { return mCurrentLifeTime; }

	const int GetDamageValue() const { return mDamageValue; }
	void SetDamageValue(const int InDamageValue) { mDamageValue = InDamageValue; }

	bool CanRespawn() { return mDefaultRespawnCoolDown > sf::Time::Zero; }
};