#pragma once
#include "Window.h"
#include "GameData/EventManager.h"
#include "GameData/TextureManager.h"

#include "ECS/Systems/SystemManager.h"
#include "ECS/EntityManager.h"

struct SharedContext{
	SharedContext():
		mWind(nullptr),
		mEventManager(nullptr),
		mTextureManager(nullptr),
		mSystemManager(nullptr),
		mEntityManager(nullptr)
		
	{}

	Window* mWind;
	EventManager* mEventManager;
	TextureManager* mTextureManager;
	SystemManager* mSystemManager;
	EntityManager* mEntityManager;
};