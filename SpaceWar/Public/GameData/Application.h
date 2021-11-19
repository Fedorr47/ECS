#pragma once

#include <memory>

#include <GameData/TextureManager.h>
#include <GameData/States/StateManager.h>

class Application
{
	static const sf::Time TimePerFrame;

	class Window* mWindow;
	//class SharedContext mContext;

	// Managers

	TextureManager	mTexturemanager;
	//class FontManager			mFontManager;
	SystemManager		mSystemManager;
	EntityManager		mEntityManager;
	//class GuiManager			mGuiManager;
	StateManager		mStateManager;
	SharedContext     mContext;

	sf::Clock mClock;
	sf::Time mElapsed;

	void Update();
	void Render();
	void PostUpdate();

	void RestartClock();

public:
	Application();
	~Application() {};

	sf::Time GetElapsed();

	void Run();
};