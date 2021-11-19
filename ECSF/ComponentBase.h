#pragma once

#include "ECS_utils.h"

class ComponentBase
{	
public:
	explicit ComponentBase(const ComponentType InType) :
		mType(InType)
	{}
private:
	ComponentType mType;
protected:
};