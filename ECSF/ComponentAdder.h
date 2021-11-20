#pragma once

#include "IComponentAdder.h"

#include <memory>
#include <unordered_map>

#include "ECS_utils.h"

using ComponentMapType = std::unordered_map<ComponentType, std::unique_ptr<IComponentBase>>;

class ComponentAdder : public IComponentAdder
{
public:
	ComponentAdder() :
		mComponentsCount(DEFAULT_MAX_COMPONENT_COUNT)
	{
		mComponents.reserve(mComponentsCount);
	}
	virtual ~ComponentAdder() {}

	IComponentAdder* GetInstance();

	bool AddComponent(std::unique_ptr<IComponentBase> InComponent) override;
	bool RemoveComponent(const ComponentType InComponentType) override;

private:
	size_t mComponentsCount;
	ComponentMapType mComponents;

	static IComponentAdder* ObjectComponentAdder;
};

extern "C" ECSF_API IComponentAdder* GetComponentAdder();