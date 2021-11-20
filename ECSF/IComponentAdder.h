#pragma once

#ifdef ECSF_EXPORTS
#define ECSF_API __declspec(dllexport)
#else
#define ECSF_API __declspec(dllimport)
#endif

#include <memory>

#include "IComponentBase.h"

ECSF_API ComponentType;

class ECSF_API IComponentAdder
{
public:
	IComponentAdder() {};
	virtual ~IComponentAdder() {};

	virtual bool AddComponent(std::unique_ptr<IComponentBase> InComponent) = 0;
	virtual bool RemoveComponent(const ComponentType InComponentType) = 0;
};