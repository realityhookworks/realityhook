#pragma once
#include "SDK.h"

class CUtil
{
public:
	void VectorTransform(const Vector& vSome, const matrix3x4& vMatrix, Vector& vOut);

	bool IsVisible(void* pLocal, void* pEntity, Vector vStart, Vector vEnd);

	bool IsKeyPressed(int i);

	bool IsHeadshotWeapon(CBaseEntity* pLocal, CBaseCombatWeapon* pWep);

	void FixMovementForUserCmd(CUserCmd* cmd, old_movement_t mov);

	void LogToParty(const char* message, bool onlyonce)
	{
		std::string dingus = "tf_party_chat\"" + std::string(message) + "\"";

		if (!onlyonce)
		{
			gInts.Engine->ClientCmd_Unrestricted(dingus.c_str());
		}
		bool wasOnce = false;
		if (onlyonce)
		{
			if (!wasOnce)
			{
				gInts.Engine->ClientCmd_Unrestricted(dingus.c_str());
				wasOnce = true;
			}
		}

	}
	float RandFloatRange(float min, float max)
	{
		return (min + 1) + (((float)rand()) / (float)RAND_MAX) * (max - (min + 1));
	}
	void LogToConsole(const char* message)
	{
		std::string console = "echo " + std::string(message);
		gInts.Engine->ClientCmd_Unrestricted(console.c_str());
	}

	PVOID InitKeyValue();
};

extern CUtil* Util;