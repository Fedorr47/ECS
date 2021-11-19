#pragma once
#include "ECS/Components/ComponentBase.h"

class ComponentState : public ComponentBase{
public:
	ComponentState(): 
		ComponentBase(EComponentType::State)
	{}

	EEntityState GetState(){ return mState; }
	void SetState(const EEntityState& InState){
		mState = InState;
	}
private:
	EEntityState mState;
};