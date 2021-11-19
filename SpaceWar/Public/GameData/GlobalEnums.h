#pragma once

enum class EDirection : uint8_t
{
	Up = 0,
	Left,
	Down,
	Right,
	MAX
};

enum class ETag : uint8_t
{
	FirstPlayer,
	SecondPlayer,
	MAX
};

enum class ECombatEntityType : uint8_t
{
	Player,
	Enemy,
	Projectile,
	Particles,
	MAX
};

enum class EProjectileType : uint8_t
{
	Rocket,
	Laser,
	MAX
};

enum class EEntityState : uint8_t
{
	Idle,
	Moving,
	Attacking,
	MarkToRemove,
	WaitRespawn
};

enum class ERenderPriority : uint8_t
{
	BackGround = 0,
	Scene,
	HUD,
	MAX
};

enum class EScoredType : uint8_t
{
	Destroy = 0,
	MAX
};

enum class EParticleType : uint8_t
{
	Propellant,
	Hit,
	Crash,
	MAX
};
