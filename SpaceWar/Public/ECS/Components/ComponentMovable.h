#pragma once
#include "ECS/Components/ComponentBase.h"

#define FULL_CIRCLE 360.0f

class ComponentMovable : public ComponentBase {
	
	float mVelocityMax;
	bool mIsInertion;
	float mAngleStep;
	float mCurrenRotation;
	float mSpeed;
	bool mImpulse;
	sf::Vector2f mVelocity;
	sf::Vector2f mAcceleration;

	void CheckVelocity()
	{
		if (std::abs(mVelocity.x) > mVelocityMax) {
			mVelocity.x = mVelocityMax * (mVelocity.x / std::abs(mVelocity.x));
		}

		if (std::abs(mVelocity.y) > mVelocityMax) {
			mVelocity.y = mVelocityMax * (mVelocity.y / std::abs(mVelocity.y));
		}
	}

public:
	ComponentMovable() :
		ComponentBase(EComponentType::Movable), mVelocityMax(0.f),
		mIsInertion(false),
		mAngleStep(5.0f),
		mCurrenRotation(0.0f),
		mSpeed(0.0f),
		mImpulse(false),
		mVelocity(sf::Vector2f(0.0f, 0.0f)),
		mAcceleration(sf::Vector2f(0.0f, 0.0f))
	{
	}

	const sf::Vector2f& GetVelocity() { return mVelocity; }
	float GetMaxVelocity() { return mVelocityMax; }
	const float GetSpeed() { return mSpeed; }
	bool IsInertion() const { return mIsInertion; }
	float GetRotation() const { return mCurrenRotation; }

	void SetSpeed(const float InSpeed) { mSpeed = InSpeed; }
	void SetVelocity(const sf::Vector2f& VelocityVec) { 
		mVelocity = VelocityVec; 
		CheckVelocity();
	}
	void SetVelocity(float InX, float InY) 
	{ 
		mVelocity = sf::Vector2f(InX, InY); 
	}
	void SetMaxVelocity(float MaxVelocity) { mVelocityMax = MaxVelocity; }
	void SetInertion(bool InState) { mIsInertion = InState; }
	void SetRotation(const float& InAngle){ mCurrenRotation = InAngle; }

	void AddVelocity(const sf::Vector2f& AdditionalVec) {
		mVelocity += AdditionalVec;
		CheckVelocity();
	}

	void Accelerate(const sf::Vector2f& AccelerateVec) { AddVelocity(AccelerateVec); }
	void Accelerate(float InX, float InY) { AddVelocity(sf::Vector2f(InX, InY)); }

	void SetImpulse(bool InState = true) { mImpulse = InState; }
	bool GetImpulse() { return mImpulse; }

	void SetRotation(const EDirection& Direction)
	{ 
		switch (Direction)
		{
		case EDirection::Left:
			mCurrenRotation -=mAngleStep;
			break;
		case EDirection::Right:
			mCurrenRotation += mAngleStep;
			break;
		}
		if (mCurrenRotation > FULL_CIRCLE)
		{
			mCurrenRotation -= FULL_CIRCLE;
		}
		else if (mCurrenRotation < 0.0f)
		{
			mCurrenRotation += FULL_CIRCLE;
		}
	}
};