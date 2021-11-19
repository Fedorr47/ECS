#pragma once
#include "ECS/Components/ComponentBase.h"

class ComponentProjectile : public ComponentBase{
	
	sf::FloatRect mChacedBounds;
	sf::Vector2f mPosition;
	float mAngle;
	sf::Time mDefaultSpawnTimeRate;
	sf::Time mCurrentSpawnTimeRate;

public:
	ComponentProjectile():
		ComponentBase(EComponentType::Projectile)
	{}

	const sf::FloatRect& GetBounds() const { return mChacedBounds; }
	void SetBounds(const sf::FloatRect& InBounds) { mChacedBounds = InBounds; }

	const sf::Vector2f& GetPosition() const { return mPosition; }
	void SetPosition(const sf::Vector2f& InPosition) { mPosition = InPosition; }

	const float GetRotation() const { return mAngle; }
	void SetRotation(const float InAnfle) { mAngle = InAnfle; }

	const sf::Time& GetDefaultSpawnTimeRate() const { return mDefaultSpawnTimeRate; }
	void SetDefaultSpawnTimeRate(const sf::Time& InTimeRate) { mDefaultSpawnTimeRate = InTimeRate; }

	const sf::Time& GetCurrentSpawnTimeRate() const { return mCurrentSpawnTimeRate; }
	void SetCurrentSpawnTimeRate(const sf::Time& InTimeRate) { mCurrentSpawnTimeRate = InTimeRate; }
};