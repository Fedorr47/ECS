#pragma once

enum class ERenderPriority : uint8_t;
enum class EResourceTag : uint8_t;

struct ActorOptions
{
	EResourceTag ResourceTag;
	ERenderPriority RenderPriority;
	bool IsInertion{ false };
	float Angle{ 0.0f };
	float Speed{ 0.0f };
	float MaxVelocity{ 0.0f };
	float LifeTime{ 0.0f };
	sf::Vector2f Position{ 0.0f,0.0f };
	unsigned DamageValue{0};
	unsigned HealthPoints{0};
	float ProjectileSpawnRate{0.0f};
	float RespawnCounter{ 0.0f };

	ActorOptions() {}
	~ActorOptions() {}
};