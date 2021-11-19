#pragma once

enum class EntityEvent : uint8_t
{
	Spawned,
	Despawned,
	Respawned,
	Colliding_X,
	Colliding_Y,
	Moving_Left,
	Moving_Right,
	Moving_Up,
	Moving_Down,
	Became_Idle,
	Began_Moving,
	BeganAttack,
	Hit,
	Destroyed,
	MAX
};