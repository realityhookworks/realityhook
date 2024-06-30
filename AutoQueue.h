#pragma once
#include "SDK.h"

class CQueue
{
public:
	void Run();
	bool IfCanqueue();
};
extern CQueue gQueue;