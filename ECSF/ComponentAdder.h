#pragma once

#include <any>
#include <memory>
#include <functional>
#include <unordered_map>
#include <optional>

#include "ECS_utils.h"
#include "IComponentBase.h"

using ComponentMapType = std::unordered_map<ComponentType, std::function<std::unique_ptr<IComponentBase>(void)>>;
using OptionalComponent = std::optional<std::unique_ptr<IComponentBase>>;

class ComponentAdder
{
public:
	ComponentAdder() :
		mComponentsCount{ DEFAULT_MAX_COMPONENT_COUNT }
	{
		mComponents.reserve(mComponentsCount);
	}
	virtual ~ComponentAdder() {
	}

	template <class TComponentType>
	bool RegisterComponentType(const ComponentType InComponentType);
	bool UnregisterComponentType(const ComponentType InComponentType);
	OptionalComponent HasComponent(const ComponentType InComponentType);

	const size_t GetComponentsCount() const { return mComponentsCount; }
	void SetComponentsCount(const size_t InComponentsCount) { mComponentsCount = InComponentsCount; }

private:
	size_t mComponentsCount;
	ComponentMapType mComponents;
};

template <class TComponentType>
bool ComponentAdder::RegisterComponentType(const ComponentType InComponentType)
{
	bool Result = false;
	if (InComponentType <= mComponentsCount)
	{
		mComponents.emplace(InComponentType, [&]() -> std::unique_ptr<IComponentBase> { return std::make_unique<TComponentType>(InComponentType); });
		Result = true;
	}

	return Result;
}

