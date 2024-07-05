#pragma once
#include "SDK.h"

class CAntiAim
{
public:
	void Run(CBaseEntity* pLocal, CUserCmd* pCommand);
};
extern CAntiAim gAntiAim;