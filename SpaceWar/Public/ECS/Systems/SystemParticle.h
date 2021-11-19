#pragma once

#include <queue>

#include "ECS/Systems/SystemBase.h"

enum class EParticleType : uint8_t;
enum class EResourceTag : uint8_t;

class SystemParticle : public SystemBase{

	std::queue<std::pair<const EntityID, EParticleType>> EntityParticlies;
	
	void EmitParticles(float DeltaTime, const EntityID EntityId, EParticleType ParticleType);

public:
	SystemParticle(SystemManager* InSystemManager);
	~SystemParticle();

	void Update(float DeltaTime);
	void HandleEvent(
		const EntityID EntityId,
		const EntityEvent EventtType);
	void Notify(const Message& msg);
	void ComputeVertices(const EntityID EntityId);

private:
	void ChangePosition(const EntityID EntityId, const sf::Vector2f& InPosVec);
	void ChangeRotation(const EntityID EntityId, const float InAngle);
	void RefreshEntityBounds(const EntityID EntityId, const sf::FloatRect& InBounds);
	void AttachParticle(const EntityID EntityId, EResourceTag ResourceTag);

	void EmitPropellant(float DeltaTime, const EntityID EntityId, class ComponentParticleEmmiter* InComponentParticleEmmiterPtr);
	void EmitHit(float DeltaTime, const EntityID EntityId, class ComponentParticleEmmiter* InComponentParticleEmmiterPtr);
	void EmitCrash(float DeltaTime, const EntityID EntityId, class ComponentParticleEmmiter* InComponentParticleEmmiterPtr);
};