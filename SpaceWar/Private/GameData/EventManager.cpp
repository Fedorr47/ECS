#include "stdafx.h"

#include "GameData/EventManager.h"
#include "GameData/States/StateManager.h"
#include <cstring>


EventManager::EventManager() : 
	mCurrentState(EStateType(0)), mHasFocus(true)
{
	LoadBindings();
}

EventManager::~EventManager()
{
}

bool EventManager::AddBinding(std::unique_ptr<Binding> Binding)
{
	if(mBindings.find(Binding->mName) != mBindings.end())
		return false;

	return mBindings.emplace(Binding->mName, std::move(Binding)).second;
}

bool EventManager::RemoveBinding(std::string Name)
{
	auto BindingItr = mBindings.find(Name);
	if (BindingItr == mBindings.end())
	{ 
		return false; 
	}
	mBindings.erase(BindingItr);
	return true;
}

void EventManager::SetCurrentState(EStateType EntityState)
{
	mCurrentState = EntityState;
}

void EventManager::HandleEvent(sf::Event& EventType) {
	// Handling SFML events.
	for (auto& BindingItr : mBindings) {
		Binding& bind = *BindingItr.second;
		for (auto& EventItr : bind.mEvents) {
			EEventType sfmlEvent = (EEventType)EventType.type;
			if (EventItr.first != sfmlEvent) { continue; }
			if (sfmlEvent == EEventType::KeyDown || sfmlEvent == EEventType::KeyUp) {
				if (EventItr.second.mCode == EventType.key.code) {
					if (bind.mDetails.mKeyCode != -1) {
						bind.mDetails.mKeyCode = EventItr.second.mCode;
					}
					++(bind.EventCount);
					break;
				}
			}
			else if (sfmlEvent == EEventType::MButtonDown || sfmlEvent == EEventType::MButtonUp) {
				if (EventItr.second.mCode == EventType.mouseButton.button) {
					// Matching event/keystroke.
					// Increase count.
					bind.mDetails.mMouse.x = EventType.mouseButton.x;
					bind.mDetails.mMouse.y = EventType.mouseButton.y;
					if (bind.mDetails.mKeyCode != -1) {
						bind.mDetails.mKeyCode = EventItr.second.mCode;
					}
					++(bind.EventCount);
					break;
				}
			}
			else {
				// No need for additional checking.
				if (sfmlEvent == EEventType::MouseWheel) {
					bind.mDetails.mMouseWheelDelta = EventType.mouseWheel.delta;
				}
				else if (sfmlEvent == EEventType::WindowResized) {
					bind.mDetails.mSize.x = EventType.size.width;
					bind.mDetails.mSize.y = EventType.size.height;
				}
				else if (sfmlEvent == EEventType::TextEntered) {
					bind.mDetails.mTextEntered = EventType.text.unicode;
				}
				++(bind.EventCount);
			}
		}
	}
}

void EventManager::Update(){
	if (!mHasFocus)
	{ 
		return;
	}
	for (auto & BindingItr : mBindings){
		Binding& bind = *BindingItr.second;
		for(auto &EventItr : bind.mEvents){
			switch(EventItr.first){
			case(EEventType::Keyboard):
				if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key(EventItr.second.mCode))){
					if (bind.mDetails.mKeyCode != -1){
						bind.mDetails.mKeyCode = EventItr.second.mCode;
					}
					++(bind.EventCount);
				}
				break;
			case(EEventType::Mouse):
				if(sf::Mouse::isButtonPressed(sf::Mouse::Button(EventItr.second.mCode))){
					if (bind.mDetails.mKeyCode != -1){
						bind.mDetails.mKeyCode = EventItr.second.mCode;
					}
					++(bind.EventCount);
				}
				break;
			}
		}

		if(bind.mEvents.size() > 0 && bind.EventCount > 0){
			auto stateCallbacks = mCallbacks.find(mCurrentState);
			auto otherCallbacks = mCallbacks.find(EStateType(0));

			if(stateCallbacks != mCallbacks.end()){
				auto callItr = stateCallbacks->second.find(bind.mName);
				if(callItr != stateCallbacks->second.end()){
					callItr->second(&bind.mDetails);
				}
			}

			if(otherCallbacks != mCallbacks.end()){
				auto callItr = otherCallbacks->second.find(bind.mName);
				if(callItr != otherCallbacks->second.end()){
					callItr->second(&bind.mDetails);
				}
			}
		}
		bind.EventCount = 0;
		bind.mDetails.Clear();
	}
}

void EventManager::LoadBindings()
{
	std::unordered_map<std::string, std::pair<EEventType, int>> bindingTable;
	
	bindingTable.emplace("Player_MoveLeft", std::pair<EEventType, int>(static_cast<EEventType>(24), 0));
	bindingTable.emplace("Player_MoveRight", std::pair<EEventType, int>(static_cast<EEventType>(24), 3));
	bindingTable.emplace("Player_MoveUp", std::pair<EEventType, int>(static_cast<EEventType>(24), 22));
	bindingTable.emplace("Player_Attack", std::pair<EEventType, int>(static_cast<EEventType>(24), 57));

	bindingTable.emplace("Second_Player_MoveLeft", std::pair<EEventType, int>(static_cast<EEventType>(24), 71));
	bindingTable.emplace("Second_Player_MoveRight", std::pair<EEventType, int>(static_cast<EEventType>(24), 72));
	bindingTable.emplace("Second_Player_MoveUp", std::pair<EEventType, int>(static_cast<EEventType>(24), 73));
	bindingTable.emplace("Second_Player_Attack", std::pair<EEventType, int>(static_cast<EEventType>(24), 75));

	bindingTable.emplace("Window_close", std::pair<EEventType, int>(static_cast<EEventType>(0), 0));
	bindingTable.emplace("Mouse_Left", std::pair<EEventType, int>(static_cast<EEventType>(9), 0));
	bindingTable.emplace("Key_Escape", std::pair<EEventType, int>(static_cast<EEventType>(5), 36));

	for (auto bindRow : bindingTable)
	{
		std::unique_ptr<Binding> bind = std::make_unique<Binding>(bindRow.first);
		EventInfo eventInfo;
		eventInfo.mCode = bindRow.second.second;
		EEventType type = static_cast<EEventType>(bindRow.second.first);
		bind->BindEvent(type, eventInfo);
		AddBinding(std::move(bind));
	}
}
