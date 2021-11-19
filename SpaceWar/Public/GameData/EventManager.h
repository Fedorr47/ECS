#pragma once
#include <vector>
#include <unordered_map>
#include <assert.h>
#include <functional>
#include <fstream>
#include <sstream>
#include <iostream>

#include <SFML/Window.hpp>

enum class EEventType : uint32_t
{
	KeyDown = sf::Event::KeyPressed,
	KeyUp = sf::Event::KeyReleased,
	MButtonDown = sf::Event::MouseButtonPressed,
	MButtonUp = sf::Event::MouseButtonReleased,
	MouseWheel = sf::Event::MouseWheelMoved,
	WindowResized = sf::Event::Resized,
	GainedFocus = sf::Event::GainedFocus,
	LostFocus = sf::Event::LostFocus,
	MouseEntered = sf::Event::MouseEntered,
	MouseLeft = sf::Event::MouseLeft,
	Closed = sf::Event::Closed,
	TextEntered = sf::Event::TextEntered,
	Keyboard = sf::Event::Count + 1, Mouse, Joystick,
	GUI_Click,
	GUI_Release, 
	GUI_Hover, 
	GUI_Leave
};

struct EventInfo
{
	EventInfo(){ mCode = 0; }
	EventInfo(int EventType){ mCode = EventType; }
	union {
		int mCode;
	};
};

struct EventDetails{
	EventDetails(const std::string& BindName)
		: mName(BindName){
		Clear();
	}
	std::string mName;

	sf::Vector2i mSize;
	sf::Uint32 mTextEntered;
	sf::Vector2i mMouse;
	int mMouseWheelDelta;
	int mKeyCode; 

	void Clear(){
		mSize = sf::Vector2i(0, 0);
		mTextEntered = 0;
		mMouse = sf::Vector2i(0, 0);
		mMouseWheelDelta = 0;
		mKeyCode = -1;
	}
};

using Events = std::vector<std::pair<EEventType, EventInfo>>;

struct Binding {
	Binding(const std::string& Name) : 
		mName(Name),
		mDetails(Name), 
		EventCount(0) {}
	~Binding() {}
	void BindEvent(EEventType EventType, EventInfo Info = EventInfo()) {
		mEvents.emplace_back(EventType, Info);
	}

	Events mEvents;
	std::string mName;
	int EventCount; // Count of events that are "happening".

	EventDetails mDetails;
};

using Bindings = std::unordered_map<std::string, std::unique_ptr<Binding>>;
// Callback container.
using CallbackContainer = std::unordered_map<std::string, std::function<void(EventDetails*)>>;
// State callback container.
enum class EStateType : uint8_t;
using Callbacks = std::unordered_map<EStateType, CallbackContainer>;


class EventManager 
{
public:
	EventManager();
	~EventManager();

	bool AddBinding(std::unique_ptr<Binding> Binding);
	bool RemoveBinding(std::string Name);

	void SetCurrentState(EStateType State);
	void SetFocus(const bool& FocusState) { mHasFocus = FocusState; }

	// Needs to be defined in the header!
	template<class T>
	bool AddCallback(EStateType State, const std::string& Name,
		void(T::* func)(EventDetails*), T* Instance)
	{
		auto itr = mCallbacks.emplace(State, CallbackContainer()).first;
		auto temp = std::bind(func, Instance, std::placeholders::_1);
		return itr->second.emplace(Name, temp).second;
	}

	bool RemoveCallback(EStateType State, const std::string& Name) {
		auto itr = mCallbacks.find(State);
		if (itr == mCallbacks.end()) { return false; }
		auto itr2 = itr->second.find(Name);
		if (itr2 == itr->second.end()) { return false; }
		itr->second.erase(Name);
		return true;
	}

	void HandleEvent(sf::Event& EventType);
	void Update();

	// Getters.
	sf::Vector2i GetMousePos(sf::RenderWindow* InWindow = nullptr) {
		return (InWindow ? sf::Mouse::getPosition(*InWindow) : sf::Mouse::getPosition());
	}
private:
	void LoadBindings();

	EStateType mCurrentState;
	Bindings mBindings;
	Callbacks mCallbacks;
	bool mHasFocus;
};