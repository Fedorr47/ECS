#include "stdafx.h"

#include "GameData/EventQueue.h"

void EventQueue::AddEvent(const EventID EventType)
{
	mQueue.push(EventType);
}

bool EventQueue::ProcessEvents(EventID& EventType)
{
	if (mQueue.empty())
	{
		return false;
	}
	EventType = mQueue.front();
	mQueue.pop();
	return true;
}

void EventQueue::Clear()
{
	while (!mQueue.empty())
	{
		mQueue.pop();
	}
}
