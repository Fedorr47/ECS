#include "stdafx.h"

#include "ECS/Systems/SystemBase.h"
#include "ECS/Systems/SystemManager.h"

SystemBase::SystemBase(const ESystemType& SystemType, SystemManager* InSystemManager) : 
	mId(SystemType), 
	mSystemManager(InSystemManager)
{}

SystemBase::~SystemBase()
{ 
	Purge(); 
}

bool SystemBase::AddEntity(const EntityID EntityId){
	if (HasEntity(EntityId))
	{
		return false; 
	}
	mEntities.emplace_back(EntityId);
	return true;
}

bool SystemBase::HasEntity(const EntityID EntityId){
	return std::find(mEntities.begin(), mEntities.end(), EntityId) != mEntities.end();
}

bool SystemBase::RemoveEntity(const EntityID EntityId){
	auto entity = std::find_if(mEntities.begin(), mEntities.end(),
		[&EntityId](EntityID& id)
		{ 
			return id == EntityId; 
		});
	if (entity == mEntities.end())
	{ 
		return false; 
	}
	mEntities.erase(entity);
	return true;
}

ESystemType SystemBase::GetId()
{ 
	return mId; 
}

bool SystemBase::FitsRequirements(const Bitmask& Bits)
{
	return std::find_if(mRequiredComponents.begin(), mRequiredComponents.end(),
		[&Bits](Bitmask& b) 
		{
			return b.Matches(Bits, b.GetMask()); 
		}) != mRequiredComponents.end();
}

void SystemBase::Purge()
{ 
	mEntities.clear(); 
}