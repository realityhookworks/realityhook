#pragma once
#include "SDK.h"

class CMisc
{
public:

	void namestealer(CBaseEntity* pEntity);

	void Run(CBaseEntity* pLocal, CUserCmd* pCommand);

private:

	//Other funcs

	void NoisemakerSpam(PVOID kv);
};

extern CMisc gMisc;