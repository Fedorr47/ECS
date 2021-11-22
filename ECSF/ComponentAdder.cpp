#include "pch.h"
#include "ComponentAdder.h"
#include "IComponentBase.h"

bool ComponentAdder::UnregisterComponentType(const ComponentType InComponentType)
{
    bool Result = false;
    ComponentMapType::iterator ItrComponent = mComponents.find(InComponentType);
    if (ItrComponent != mComponents.end())
    {
        Result = true;
    }

    return Result;
}

OptionalComponent ComponentAdder::HasComponent(const ComponentType InComponentType)
{
    OptionalComponent OutComponent;
    ComponentMapType::iterator ItrComponents = mComponents.find(InComponentType);
    if (ItrComponents != mComponents.end())
    {
        OutComponent = ItrComponents->second();
    }
    return OutComponent;
}
