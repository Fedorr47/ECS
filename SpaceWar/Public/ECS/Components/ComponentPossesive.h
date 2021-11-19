#pragma once
#include "ECS/Components/ComponentBase.h"

class ComponentPossesive : public ComponentBase{
	EntityID mOwnerEntity{0};
	std::vector<EntityID> mChildEntities;
public:
	ComponentPossesive():
		ComponentBase(EComponentType::Posses)
	{}

	void SetOwnerEntity(const EntityID InEntityID) { mOwnerEntity = InEntityID; }
	const EntityID GetOwnerEntity() const { return mOwnerEntity; }

	void AddChild(const EntityID InEntityID) { mChildEntities.push_back(InEntityID); }
	const std::vector<EntityID>& GetChilds() const { return mChildEntities; }
	void RemoveChild(const EntityID InEntityID) {
		auto itr = std::find(mChildEntities.begin(), mChildEntities.end(), InEntityID);
		mChildEntities.erase(itr);
	}
};