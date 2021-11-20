#pragma once

#ifdef ECSF_EXPORTS
#define ECSF_API __declspec(dllexport)
#else
#define ECSF_API __declspec(dllimport)
#endif

#include "ECS_utils.h"

class ECSF_API IComponentBase
{
public:
	explicit IComponentBase(const ComponentType InType) :
		mType(InType)
	{}
	
	virtual const ComponentType GetType() const = 0;

protected:
	ComponentType mType;
};