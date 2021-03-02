#pragma once
#include <map>

class State
{
public:
	virtual void Update(float dt){};
	virtual void HandleInput(){};
	virtual void Enter(void** args){};
	virtual void Exit(){};
	State();
	~State();
};

class EmptyState : public State
{
public:
	void Update(float dt){};
	void HandleInput(){};
	void Enter(void** args){};
	void Exit(){};
	EmptyState();
	~EmptyState();
};

class StateMachine
{
	std::map<const char*, State*> stateMap;
	State* cur = new EmptyState();

public:
	State* Current(){ return cur; }
	void Add(const char* id, State* state) { stateMap.emplace(id, state); }
	void Remove(const char* id) { stateMap.erase(id); }
	void Clear() { stateMap.clear(); }


	void Change(const char* id, void** args)
	{
		State* next = stateMap.at(id);
		cur->Exit();
		next->Enter(args);
		cur = next;
	}

	void Update(float dt) { cur->Update(dt); }

	void HandleInput() { cur->HandleInput(); }
	
	State* GetState(const char* stateName) { return stateMap[stateName]; }
	StateMachine();
	~StateMachine();
};