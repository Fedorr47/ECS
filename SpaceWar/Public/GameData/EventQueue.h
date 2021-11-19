#pragma once
#include <queue>

using EventID = unsigned int;

class EventQueue
{
	std::queue<EventID> mQueue;

public:

	void AddEvent(const EventID EventType);
	bool ProcessEvents(EventID& EventType);
	void Clear();
};