#include "stdafx.h"

#include "ECS/ECS_Types.h"
#include "ECS/Systems/SystemManager.h"
#include "ECS/EntityManager.h"
#include "ECS/MessageHandler.h"
#include "GameData/SharedContext.h"

#include "ECS/Systems/SystemBase.h"
#include "ECS/Systems/SystemState.h"
#include "ECS/Systems/SystemRender.h"
#include "ECS/Systems/SystemControl.h"
#include "ECS/Systems/SystemMovement.h"
#include "ECS/Systems/SystemSpawn.h"
#include "ECS/Systems/SystemCollision.h"
#include "ECS/Systems/SystemParticle.h"
#include "ECS/Systems/SystemPosition.h"
#include "ECS/Systems/SystemLifetime.h"
#include "ECS/Systems/SystemHealth.h"
#include "ECS/Systems/SystemDamage.h"
#include "ECS/Systems/SystemProjectile.h"
#include "ECS/Systems/SystemRespawn.h"
#include "ECS/Systems/SystemPosses.h"
#include "ECS/Systems/SystemScore.h"
#include "GameData/EventQueue.h"

SystemManager::SystemManager() : 
	mEntityManager(nullptr)
{
	mSystems[ESystemType::State] =		std::make_unique<SystemState>(this);
	mSystems[ESystemType::Control] =	std::make_unique<SystemControl>(this);
	mSystems[ESystemType::Movement] =	std::make_unique<SystemMovement>(this);
	mSystems[ESystemType::Renderer] =	std::make_unique<SystemRender>(this);
	mSystems[ESystemType::Spawn] =		std::make_unique<SystemSpawn>(this);
	mSystems[ESystemType::Collision] =	std::make_unique<SystemCollision>(this);
	mSystems[ESystemType::Particle] =	std::make_unique<SystemParticle>(this);
	mSystems[ESystemType::Position] =	std::make_unique<SystemPosition>(this);
	mSystems[ESystemType::Lifetime] =	std::make_unique<SystemLifetime>(this);
	mSystems[ESystemType::Health] =		std::make_unique<SystemHealth>(this);
	mSystems[ESystemType::Damage] =		std::make_unique<SystemDamage>(this);
	mSystems[ESystemType::Projectile] = std::make_unique<SystemProjectile>(this); 
	mSystems[ESystemType::Respawn] =	std::make_unique<SystemRespawn>(this);
	mSystems[ESystemType::Posses] =		std::make_unique<SystemPosses>(this);
	mSystems[ESystemType::Score] =		std::make_unique<SystemScore>(this);
}

SystemManager::~SystemManager() 
{
	PurgeSystems();
}

void SystemManager::SetSharedConxtext(SharedContext* InSharedContext)
{
	mSharedContext = InSharedContext;
}

SharedContext* SystemManager::GetSharedConxtext()
{
	return mSharedContext;
}

void SystemManager::SetEntityManager(EntityManager* InEntityMgr)
{
	if (!mEntityManager) 
	{ 
		mEntityManager = InEntityMgr; 
	}
}

EntityManager* SystemManager::GetEntityManager() 
{
	return mEntityManager;
}

MessageHandler* SystemManager::GetMessageHandler() 
{
	return &m_messages;
}

void SystemManager::AddEvent(
	const EntityID& EntityId, 
	const EventID& EventType)
{
	mEvents[EntityId].AddEvent(EventType);
}

void SystemManager::Update(float DeltaTime) 
{
	for (auto& SystemItr : mSystems)
	{
		SystemItr.second->Update(DeltaTime);
	}
	HandleEvents();
}

void SystemManager::HandleEvents() 
{
	for (auto& Event : mEvents) {
		EventID id = 0;
		while (Event.second.ProcessEvents(id))
		{
			for (auto& SystemItr : mSystems)
			{
				if (SystemItr.second->HasEntity(Event.first)) {
					SystemItr.second->HandleEvent(Event.first, (EntityEvent)id);
				}
			}
		}
	}
}

void SystemManager::Draw(Window* InWindow)
{
	SystemRender* System = GetSystem<SystemRender>(ESystemType::Renderer);
	if (System == nullptr)
	{ 
		return; 
	}
	System->Render(InWindow);
}

void SystemManager::EntityModified(
	const EntityID& EntityId, 
	const Bitmask& Bits)
{
	for (auto& SystemItr : mSystems) {
		SystemBase& System = *SystemItr.second;
		if (System.FitsRequirements(Bits)) {
			if (!System.HasEntity(EntityId)) {
				System.AddEntity(EntityId);
			}
		}
		else {
			if (System.HasEntity(EntityId)) {
				System.RemoveEntity(EntityId);
			}
		}
	}
}

void SystemManager::RemoveEntity(const EntityID& EntityId) 
{
	for (auto& SystemItr : mSystems) {
		SystemItr.second->RemoveEntity(EntityId);
	}
}

void SystemManager::PurgeEntities() 
{
	for (auto& SystemItr : mSystems) 
	{
		SystemItr.second->Purge();
	}
}

void SystemManager::PurgeSystems() {
	mSystems.clear();
}