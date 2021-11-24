#pragma once

#include <any>
#include <memory>
#include <functional>
#include <optional>
#include <unordered_map>

#include "ECS_utils.h"
#include "IComponentBase.h"
#include "Allocators/StackAllocator.h"

using OptionalComponent = std::optional<std::unique_ptr<IComponentBase>>;
// TODO: change to custom hash table
using ComponentMapType = std::unordered_map<ComponentType, std::function<std::unique_ptr<IComponentBase>(void)>>;

class ComponentAdder
{
public:
	ComponentAdder() :
		mComponentsCount{ DEFAULT_MAX_COMPONENT_TYPES_COUNT }
	{
	}
	virtual ~ComponentAdder() {
	}

	template <class TComponentClassType>
	bool RegisterComponentType(const ComponentType InComponentType);
	bool UnregisterComponentType(const ComponentType InComponentType);
	OptionalComponent HasComponent(const ComponentType InComponentType);

	const size_t GetComponentsCount() const { return mComponentsCount; }
	void SetComponentsCount(const size_t InComponentsCount) { mComponentsCount = InComponentsCount; }

private:
	size_t mComponentsCount;
	ComponentMapType  mRegistredComponent;
	// TODO: custom vector for all types of registred components
	
};

template <class TComponentClassType>
bool ComponentAdder::RegisterComponentType(const ComponentType InComponentType)
{
	bool Result = false;
	if (InComponentType <= mComponentsCount)
	{
		mComponents.emplace(InComponentType, [&]() -> std::unique_ptr<IComponentBase> 
			{ 
				return std::make_unique<TComponentType>(InComponentType); 
			}
		);
	}

	return Result;
}

