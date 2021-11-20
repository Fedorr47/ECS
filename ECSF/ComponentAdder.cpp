#include "pch.h"
#include "ComponentAdder.h"

IComponentAdder* ComponentAdder::ObjectComponentAdder = nullptr;

IComponentAdder* ComponentAdder::GetInstance()
{
    if (ObjectComponentAdder == nullptr)
    {
        ObjectComponentAdder = new ComponentAdder();
    }
    return ObjectComponentAdder;
}

bool ComponentAdder::AddComponent(std::unique_ptr<IComponentBase> InComponent)
{
    bool Result = false;
    if (InComponent->GetType() < mComponentsCount)
    {
        mComponents.emplace(InComponent->GetType(), std::move(InComponent));
        Result = true;
    }
 
    return Result;
}

bool ComponentAdder::RemoveComponent(const ComponentType InComponentType)
{
    bool Result = false;
    ComponentMapType::iterator ItrComponent = mComponents.find(InComponentType);
    if (ItrComponent != mComponents.end())
    {
        mComponents.erase(ItrComponent);
        Result = true;
    }

    return Result;
}

ECSF_API IComponentAdder* GetComponentAdder()
{
    IComponentAdder* ObjComponentAdder = new ComponentAdder();
    return ObjComponentAdder;
}
