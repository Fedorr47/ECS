#pragma once
#include "ECS/Components/ComponentBase.h"

class ComponentScore : public ComponentBase{
	int mScore{0};
public:
	ComponentScore():
		ComponentBase(EComponentType::Score)
	{}

	void SetScore(const int InScore) { mScore = InScore; }
	const int GetScore() const { return mScore; }
};