#pragma once
#pragma once

#include "ECS_utils.h"

class IComponentBase
{
public:
	explicit IComponentBase(const ComponentType InType) :
		mType(InType)
	{}
	
	virtual const ComponentType GetType() const = 0;

private:
	ComponentType mType;
};