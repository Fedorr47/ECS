#pragma once

class StateManager;
class BaseState{
friend class StateManager;
public:
	BaseState(StateManager* InStateManager)
		:mStateMgr(InStateManager), 
		mTransparent(false),
		mTranscendent(false){}
	virtual ~BaseState(){}

	virtual void OnCreate() = 0;
	virtual void OnDestroy() = 0;

	virtual void Activate() = 0;
	virtual void Deactivate() = 0;

	virtual void Update(const sf::Time& DeltaTime) = 0;
	virtual void Draw() = 0;

	void SetTransparent(const bool& InTransparent){ mTransparent = InTransparent; }
	bool IsTransparent()const{ return mTransparent; }
	void SetTranscendent(const bool& InTranscendence){ mTranscendent = InTranscendence; }
	bool IsTranscendent()const{ return mTranscendent; }
	sf::View& GetView(){ return mView; }
	StateManager* GetStateManager(){ return mStateMgr; }
protected:
	StateManager* mStateMgr;
	bool mTransparent;
	bool mTranscendent;
	sf::View mView;
};