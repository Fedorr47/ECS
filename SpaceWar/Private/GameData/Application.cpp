#include "stdafx.h"

#include "GameData/Application.h"
#include "GameData/Window.h"

#include <ECS/EntityManager.h>
#include <ECS/Systems/SystemManager.h>

const sf::Time Application::TimePerFrame = sf::seconds(1.f / 60.f);

Application::Application() :
	mEntityManager(&mSystemManager, &mTexturemanager),
	mStateManager(&mContext)
{
	mWindow = Window::GetInstance("SpaceWar");

	mSystemManager.SetEntityManager(&mEntityManager);

	mContext.mWind = mWindow;
	mContext.mEventManager = mWindow->GetEventManager();
	mContext.mTextureManager = &mTexturemanager;
	
	mContext.mSystemManager = &mSystemManager;
	mContext.mEntityManager = &mEntityManager;

	mStateManager.SwitchTo(EStateType::MainMenu);
}

sf::Time Application::GetElapsed()
{
	return mClock.getElapsedTime();
}

void Application::Run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	while (!mWindow->IsDone())
	{
		sf::Time dt = clock.restart();
		timeSinceLastUpdate += dt;

		while (timeSinceLastUpdate > TimePerFrame)
		{
			timeSinceLastUpdate -= TimePerFrame;
			Update();
			PostUpdate();
		}
		Render();
	}
}

void Application::Update()
{
	mWindow->Update();
	mStateManager.Update(GetElapsed());
}

void Application::Render()
{
	mWindow->BeginDraw();
	mStateManager.Draw();

	//sf::View CurrentView = mWindow->GetRenderWindow().getView();
	//mWindow->GetRenderWindow().setView(mWindow->GetRenderWindow().getDefaultView());
	//mWindow->GetRenderWindow().setView(CurrentView);

	mWindow->EndDraw();
}

void Application::PostUpdate()
{
	mStateManager.ProcessRequests();
	RestartClock();
}

void Application::RestartClock()
{
	mElapsed = mClock.restart();
}

