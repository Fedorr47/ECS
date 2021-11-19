#pragma once

#include "GameData/EventManager.h"

class Window
{	
	sf::Vector2u mSize;
	std::string mTitle;
	bool mIsDone {false};
	bool mIsFullScreen {false};
	bool mIsFocused {false};
	sf::RenderWindow mWindow;
	EventManager mEvenetManager;

public:

	static Window* GetInstance(const std::string& Title = "Window", const sf::Vector2u& Size = sf::Vector2u(640,480), sf::Uint32 Style = 0);
	~Window();
	
	void Create();
	void Update();

	void BeginDraw();
	void EndDraw();
	
	bool IsDone() const { return mIsDone; };
	void Close(EventDetails* Details = nullptr) { mIsDone = true; }
	sf::Vector2u GetSize() { return mSize; }
	sf::RenderWindow& GetRenderWindow() { return mWindow; }
	EventManager* GetEventManager() { return &mEvenetManager; }

protected:

	Window(const std::string& Title, const sf::Vector2u& Size, sf::Uint32 Style);

	static Window* mWindowInstance;
};