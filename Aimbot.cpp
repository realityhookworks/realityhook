#include "Aimbot.h"
#include "Util.h"

CAimbot gAim;

void CAimbot::Run(CBaseEntity* pLocal, CUserCmd* pCommand)
{
	old_movement_t old_mov = old_movement_t();

	if (gCvars.aimbot_silent) { // only backup the c_usercmd data when it's needed.
		old_mov.angle = pCommand->viewangles;
		old_mov.fwd = pCommand->forwardmove;
		old_mov.sdm = pCommand->sidemove;
	}

	gCvars.iAimbotIndex = -1;

	if (!gCvars.aimbot_active)
		return;
	if (!pLocal->GetActiveWeapon())
		return;

	CBaseEntity* pEntity = GetBaseEntity(GetBestTarget(pLocal, pCommand));

	if (!pEntity)
		return;

	iBestHitbox = GetBestHitbox(pLocal, pEntity);

	if (iBestHitbox == -1)
		return;

	Vector vEntity = pEntity->GetHitboxPosition(iBestHitbox);

	Vector vLocal = pLocal->GetEyePosition();

	VectorAngles((vEntity - vLocal), vAngs);

	ClampAngle(vAngs);
	gCvars.iAimbotIndex = pEntity->GetIndex();

	pCommand->viewangles = vAngs; // always set this cuz otherwise the viewangles will desync.

	if (!gCvars.aimbot_silent) {
		gInts.Engine->SetViewAngles(pCommand->viewangles);
	}

	if (gCvars.aimbot_silent) { // apply our movement fix if silent aim is enabled.
		Util->FixMovementForUserCmd(pCommand, old_mov);
	}
	if (gCvars.aimbot_autoscope && !pLocal->IsScoped() && pLocal->szGetClass() == "Sniper")
	{
		pCommand->buttons |= IN_ATTACK2;
	}
	if (pLocal->GetActiveWeapon()->GetItemDefinitionIndex() == sniperweapons::WPN_HitmanHeatmaker)
	{
		pCommand->buttons |= IN_RELOAD;
	}
	if (!gCvars.iAimbotIndex && pLocal->IsScoped())
	{
		// if we are scoped and we dont have any person unscope.
		pCommand->buttons |= IN_ATTACK2;
	}
	if (gCvars.aimbot_autoshoot)
	{
		float flCurTime = gInts.Engine->Time();
		static float flNextSend = 0.0f;
		if (pLocal->szGetClass() == "Sniper")
		{
			if (!gCvars.aimbot_zoomedonly)
			{
				if (flCurTime > flNextSend)
				{
					pCommand->buttons |= IN_ATTACK;
					flNextSend = (flCurTime + 0.2f); // this is retarded but fuck it 
				}
			}
			if (gCvars.aimbot_zoomedonly)
			{
				if (pLocal->GetCond() & TFCond_Zoomed) // if scoped
				{
					if (flCurTime > flNextSend)
					{
						pCommand->buttons |= IN_ATTACK;
						flNextSend = (flCurTime + 0.2f);
					}
				}
			}
		}
		if (pLocal->szGetClass() != "Sniper") // just shoot normally if the class isnt sniper
		{
			pCommand->buttons |= IN_ATTACK;
		}
	}
}
Vector CAimbot::calc_angle(Vector src, Vector dst)
{
	Vector AimAngles, delta;
	float hyp;
	delta = src - dst;
	hyp = sqrtf((delta.x * delta.x) + (delta.y * delta.y)); //SUPER SECRET IMPROVEMENT CODE NAME DONUT STEEL
	AimAngles.x = atanf(delta.z / hyp) * RADPI;
	AimAngles.y = atanf(delta.y / delta.x) * RADPI;
	AimAngles.z = 0.0f;
	if (delta.x >= 0.0)
		AimAngles.y += 180.0f;
	return AimAngles;
}
void CAimbot::MakeVector(Vector angle, Vector& vector)
{
	float pitch, yaw, tmp;
	pitch = float(angle[0] * PI / 180);
	yaw = float(angle[1] * PI / 180);
	tmp = float(cos(pitch));
	vector[0] = float(-tmp * -cos(yaw));
	vector[1] = float(sin(yaw) * tmp);
	vector[2] = float(-sin(pitch));
}
float CAimbot::GetFOV(Vector angle, Vector src, Vector dst)
{
	Vector ang, aim;
	float mag, u_dot_v;
	ang = calc_angle(src, dst);


	MakeVector(angle, aim);
	MakeVector(ang, ang);

	mag = sqrtf(pow(aim.x, 2) + pow(aim.y, 2) + pow(aim.z, 2));
	u_dot_v = aim.Dot(ang);

	return RAD2DEG(acos(u_dot_v / (pow(mag, 2))));
}
int CAimbot::GetBestTarget(CBaseEntity* pLocal, CUserCmd* pCmd)
{
	int iBestTarget = -1;
						 //this num could be smaller 
	float flDistToBest = 99999.f;
	int minimalDistance = INT_MAX;

	Vector vLocal = pLocal->GetEyePosition();

	for (int i = 1; i <= gInts.Engine->GetMaxClients(); i++)
	{
		if (i == me)
			continue;

		CBaseEntity* pEntity = GetBaseEntity(i);

		if (!pEntity)
			continue;

		if (pEntity->IsDormant())
			continue;

		if (pEntity->GetLifeState() != LIFE_ALIVE ||
			pEntity->GetTeamNum() == pLocal->GetTeamNum())
			continue;

		int iBestHitbox = GetBestHitbox(pLocal, pEntity);

		if (iBestHitbox == -1)
			continue;

		Vector vEntity = pEntity->GetHitboxPosition(iBestHitbox); //pEntity->GetWorldSpaceCenter(vEntity);

		if (!gCvars.PlayerMode[i])
			continue;

		if (pEntity->GetCond() & TFCond_Ubercharged ||
			pEntity->GetCond() & TFCond_Bonked)
			continue;

		//float flDistToTarget = (vLocal - vEntity).Length();
		float flFOV = GetFOV(pCmd->viewangles, vLocal, vEntity);
		float distance = flGetDistance(vEntity, pLocal->GetEyePosition());

		if (distance < minimalDistance)//gCvars.aimbot.fov)
		{
			if (flFOV < flDistToBest && flFOV < gCvars.aimbot_fov)
			{
				if (gCvars.PlayerMode[i] == 2)
					return i;
				//flDistToBest = flDistToTarget;
				flDistToBest = flFOV;
				gCvars.iAimbotIndex = i;
				iBestTarget = i;
			}
		}

		if (gCvars.PlayerMode[i] == 2) //always aim at rage targets first
			return i;
	}

	return iBestTarget;
}

int CAimbot::GetBestHitbox(CBaseEntity* pLocal, CBaseEntity* pEntity)
{
	int iBestHitbox = -1;

	if (!gCvars.aimbot_hitbox)
	{
		if (Util->IsHeadshotWeapon(pLocal, pLocal->GetActiveWeapon()))
			iBestHitbox = 0;
		else
			iBestHitbox = 4;
	}
	else
	{
		iBestHitbox = gCvars.aimbot_hitbox - 1;
	}

	if (gCvars.aimbot_hitscan)
	{
		for (int i = 0; i < 17; i++)
		{
			if (Util->IsVisible(pLocal, pEntity, pLocal->GetEyePosition(), pEntity->GetHitboxPosition(i)))
				return i;
		}
	}

	if (pEntity->GetHitboxPosition(iBestHitbox).IsZero())
		return -1;

	if (!Util->IsVisible(pLocal, pEntity, pLocal->GetEyePosition(), pEntity->GetHitboxPosition(iBestHitbox)))
		return -1;
	
	return iBestHitbox;
}