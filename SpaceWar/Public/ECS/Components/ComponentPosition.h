#pragma once

#include "ECS/Components/ComponentBase.h"

class ComponentPosition : public ComponentBase
{
	sf::Vector2f mCurrentPosition;
	sf::Vector2f mPreviousPosition;
	sf::FloatRect mChacedBounds;
	float mCurrenRotation{ 0.0f };

public:

	ComponentPosition() :
		ComponentBase(EComponentType::Position),
		mCurrentPosition(sf::Vector2f(0.0f, 0.0f)),
		mPreviousPosition(sf::Vector2f(0.0f, 0.0f))
	{}
	~ComponentPosition() {}

	const sf::Vector2f& GetCurrentPosition() { return mCurrentPosition; }
	const sf::Vector2f& GetPreviousPosition() { return mPreviousPosition; }
	const float GetRotation() const { return mCurrenRotation; }

	void SetPosition(const sf::Vector2f& InVec)
	{
		mPreviousPosition = mCurrentPosition;
		mCurrentPosition = InVec;
	}

	void SetPosition(float InX, float InY)
	{
		mPreviousPosition = mCurrentPosition;
		mCurrentPosition = sf::Vector2f(InX, InY);
	}

	void Move(float InX, float InY)
	{
		mPreviousPosition = mCurrentPosition;
		mCurrentPosition += sf::Vector2f(InX, InY);
	}

	void Move(const sf::Vector2f& InVec)
	{
		mPreviousPosition = mCurrentPosition;
		mCurrentPosition += InVec;
	}

	void SetRotation(float InAngle) { mCurrenRotation = InAngle; }

	const sf::FloatRect& GetBounds() const { return mChacedBounds; }
	void SetBounds(const sf::FloatRect& InBounds) { mChacedBounds = InBounds; }
};