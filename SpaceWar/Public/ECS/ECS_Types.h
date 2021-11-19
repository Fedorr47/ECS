#pragma once

#include "ECS/Bitmask.h"

#define DEFAULT_ENTITY -1

static const size_t MAX_COMPONENTS = 64;

using ComponentType = unsigned int;
using EntityID = unsigned int;

enum class EComponentType : uint8_t
{
	Position = 0,
	Sprite,
	State,
	Movable,
	Controller,
	Collidable,
	Particle,
	Lifetime,
	Health,
	Damage,
	Respawn,
	Projectile,
	Posses,
	Score,
	MAX
};

enum class ESystemType : uint8_t
{
	Renderer = 0, 
	Movement, 
	Collision, 
	Control, 
	State, 
	Spawn,
	Particle,
	Position,
	Lifetime,
	Health,
	Damage,
	Projectile,
	Respawn,
	Posses,
	Score,
	MAX
};