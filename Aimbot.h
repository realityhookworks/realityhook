#pragma once
#include "SDK.h"

class CAimbot
{
public:

	void Run(CBaseEntity* pLocal, CUserCmd* pCommand);

	Vector calc_angle(Vector src, Vector dst);
	int iBestHitbox;
	Vector vAngs;


	void MakeVector(Vector angle, Vector& vector);

	float GetFOV(Vector angle, Vector src, Vector dst);

	int GetBestTarget(CBaseEntity* pLocal, CUserCmd* pCmd);
	float flGetDistance(Vector vOrigin, Vector vLocalOrigin)
	{
		Vector vDelta = vOrigin - vLocalOrigin;

		float m_fDistance = sqrt(vDelta.Length());

		if (m_fDistance < 1.0f)
			return 1.0f;

		return m_fDistance;
	}
	//int GetBestTarget(CBaseEntity* pLocal);

	int GetBestHitbox(CBaseEntity* pLocal, CBaseEntity* pEntity);
};

extern CAimbot gAim; //dsajkhfdlksahf