#pragma once

#include <vector>

#include "Observer.h"

using ObserverContainer = std::vector<Observer*>;

class Communicator{

	ObserverContainer mObservers;

public:
	~Communicator()
	{ 
		mObservers.clear(); 
	}

	bool AddObserver(Observer* observer)
	{
		if (HasObserver(observer))
		{ 
			return false; 
		}
		mObservers.emplace_back(observer);
		return true;
	}

	bool RemoveObserver(Observer* observer)
	{
		auto observerItr = std::find_if(mObservers.begin(), mObservers.end(),
			[&observer](Observer* o)
			{ 
				return o == observer;
			}
		);
		if (observerItr == mObservers.end())
		{ 
			return false;
		}
		mObservers.erase(observerItr);
		return true;
	}

	bool HasObserver(const Observer* observer){
		return (std::find_if(mObservers.begin(), mObservers.end(),
			[&observer](Observer* o)
			{ 
				return o == observer;
			}) != mObservers.end());
	}

	void Broadcast(const Message& msg)
	{
		for(auto& itr : mObservers)
		{ 
			itr->Notify(msg);
		}
	}
};