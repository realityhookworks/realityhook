#pragma once
#include "SDK.h"


class CListener : public IGameEventListener2
{
public:

	//void Run(CBaseEntity * pLocal, CUserCmd * pCommand);

	void Init();

	void FireGameEvent(IGameEvent* event);

	bool wasEventInitialized = false;
	/*
	std::string repeat(int n, const char* str)
	{
		std::ostringstream os;
		for (int i = 0; i < n; i++)
			os << str;
		return os.str();
	};
	*/
};

extern CListener gListener;