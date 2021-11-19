#pragma once

#include <unordered_map>

#include "Communicator.h"
#include "ECS/EntityMessages.h"

using Subscribtions = std::unordered_map<EntityMessage,Communicator>;

class MessageHandler{
public:
	bool Subscribe(const EntityMessage& Type, Observer* observer)
	{
		return mCommunicators[Type].AddObserver(observer);
	}

	bool Unsubscribe(const EntityMessage& Type, Observer* observer)
	{
		return mCommunicators[Type].RemoveObserver(observer);
	}

	void Dispatch(const Message& msg)
	{
		auto itr = mCommunicators.find((EntityMessage)msg.mType);
		if (itr == mCommunicators.end()){ return; }
		itr->second.Broadcast(msg);
	}


private:
	Subscribtions mCommunicators;
};