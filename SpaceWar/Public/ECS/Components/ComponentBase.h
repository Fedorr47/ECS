#pragma once

#include "ECS\ECS_Types.h"

class ComponentBase
{
public:
	explicit ComponentBase(const EComponentType& Type)
		: mType(Type)
	{}

	virtual ~ComponentBase() {}

	EComponentType GetType() { return mType; }

protected:
	EComponentType mType;
};
