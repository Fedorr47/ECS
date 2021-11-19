#include "stdafx.h"

#include "ECS/EntityManager.h"
#include "ECS/Components/ComponentPosition.h"
#include "ECS/Components/ComponentState.h"
#include "ECS/Components/ComponentSprite.h"
#include "ECS/Components/ComponentMovable.h"
#include "ECS/Components/ComponentCollidable.h"
#include "ECS/Components/ComponentParticleEmmiter.h"
#include "ECS/Components/ComponentLifetime.h"
#include "ECS/Components/ComponentDamage.h"
#include "ECS/Components/ComponentHealth.h"
#include "ECS/Components/ComponentProjectile.h"
#include "ECS/Components/ComponentRespawn.h"
#include "ECS/Components/ComponentPossesive.h"
#include "ECS/Components/ComponentScore.h"
#include "GameData/EntityEvents.h"
#include "GameData/EventQueue.h"
#include "GameData/TextureManager.h"
#include "ECS/Systems/SystemManager.h"

EntityManager::EntityManager(
    SystemManager* InSystemManager, 
    TextureManager* InTextureManager
) :
    mSystems(InSystemManager),
    mTextureManager(InTextureManager)
{
    AddComponentType<ComponentPosition>(EComponentType::Position);
    AddComponentType<ComponentSprite>(EComponentType::Sprite);
    AddComponentType<ComponentState>(EComponentType::State);
    AddComponentType<ComponentMovable>(EComponentType::Movable);
    AddComponentType<ComponentCollidable>(EComponentType::Collidable);
    AddComponentType<ComponentParticleEmmiter>(EComponentType::Particle);
    AddComponentType<ComponentLifetime>(EComponentType::Lifetime);
    AddComponentType<ComponentHealth>(EComponentType::Health);
    AddComponentType<ComponentDamage>(EComponentType::Damage);
    AddComponentType<ComponentProjectile>(EComponentType::Projectile);
    AddComponentType<ComponentRespawn>(EComponentType::Respawn);
    AddComponentType<ComponentPossesive>(EComponentType::Posses);
    AddComponentType<ComponentScore>(EComponentType::Score);
}

EntityID EntityManager::AddEntity(const Bitmask& Mask)
{
    EntityID EntityID = mIdCounter;
    EntityData Data = std::make_pair<Bitmask, ComponentContainer>(0, ComponentContainer());
    auto Emplaced = mEntities.emplace(EntityID, Data);
    if (!Emplaced.second)
    {
        return -1;
    }

    ++mIdCounter;

    for (unsigned int i = 0; i < MAX_COMPONENTS; ++i)
    {
        if (Mask.test(i))
        {
            AddComponent(EntityID, static_cast<EComponentType>(i));
        }
    }

    mSystems->EntityModified(EntityID, Mask);
    mSystems->AddEvent(EntityID, (EventID)EntityEvent::Spawned);

    return EntityID;
}

bool EntityManager::RemoveEntity(const EntityID EntityId)
{
    auto EntityItr = mEntities.find(EntityId);

    if (EntityItr == mEntities.end())
    {
        return false;
    }

    ComponentContainer& Container = EntityItr->second.second;
    for (ComponentBase* Component : Container)
    {
        delete Component;
        Container.pop_back();
    }

    mEntities.erase(EntityItr);

    mSystems->RemoveEntity(EntityId);

    return true;
}

bool EntityManager::AddComponent(
    const EntityID EntityId, 
    const EComponentType ComponentType)
{
    auto EntityItr = mEntities.find(EntityId);
    if (EntityItr == mEntities.end())
    {
        return false;
    }
    Bitmask Mask = EntityItr->second.first;

    if (Mask.test(static_cast<unsigned int>(ComponentType)))
    {
        return false;
    }

    auto ComponentItr = mComponentFactory.find(ComponentType);
    if (ComponentItr == mComponentFactory.end()) 
    { 
        return false; 
    }

    ComponentBase* Component = ComponentItr->second();

    EntityItr->second.second.emplace_back(Component);
    EntityItr->second.first.set(static_cast<unsigned int>(ComponentType));
  
    mSystems->EntityModified(EntityId, EntityItr->second.first);

    return true;
}

bool EntityManager::RemoveComponent(
    const EntityID EntityId,
    const EComponentType ComponentType)
{
    auto EntityItr = mEntities.find(EntityId);
    if (EntityItr == mEntities.end())
    {
        return false;
    }
    Bitmask Mask = EntityItr->second.first;

    if (Mask.test(static_cast<unsigned int>(ComponentType)))
    {
        return false;
    }

    auto& Container = EntityItr->second.second;

    auto Component = std::find_if(Container.begin(), Container.end(),
        [&ComponentType](ComponentBase* component) 
        { 
            return component->GetType() == ComponentType; 
        }
    );

    if (Component == Container.end())
    { 
        return false; 
    }

    delete (*Component);

    Container.erase(Component);

    Mask.ClearBit(static_cast<unsigned int>(ComponentType));

    mSystems->EntityModified(EntityId, Mask);

    return true;
}

bool EntityManager::HasComponent(
    const EntityID EntityId, 
    const EComponentType ComponentType)
{
    auto EntityItr = mEntities.find(EntityId);
    if (EntityItr == mEntities.end())
    {
        return false;
    }
    Bitmask Mask = EntityItr->second.first;

    return Mask.test(static_cast<unsigned int>(ComponentType));
}

void EntityManager::Purge()
{
    mSystems->PurgeEntities();

    for (auto& Entity : mEntities) {
        Bitmask EntityMask = Entity.second.first;
        std::vector<ComponentBase*> EntityComponents = Entity.second.second;

        for (ComponentBase* Component : EntityComponents)
        {
            delete Component;
            EntityComponents.pop_back();
        }
       
        EntityMask.reset();
        EntityComponents.clear();
    }

    mEntities.clear();
    mIdCounter = 0;
}
