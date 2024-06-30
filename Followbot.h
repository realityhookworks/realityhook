#pragma once
#include "SDK.h"
#include <mutex>
#include <deque>
class CFollow
{
public:
	
	void Run(CBaseEntity* pLocal, CUserCmd* pCmd);
};
/*
#include "SDK.h"
#include "FollowBot.h"
#include "CDrawManager.h"
#include "Vector.h"
#include "Util.h"
// This "followBot" is trash but it kinda works I guess.

CFollow gFollow;
Vector scn;

std::pair<float, float> ComputeMove(const Vector& a, const Vector& b, CUserCmd* pCommand)
{
    Vector diff = (b - a);
    if (diff.Length() == 0)
        return { 0, 0 };
    const float x = diff.x;
    const float y = diff.y;
    Vector vsilent(x, y, 0);
    float speed = sqrt(vsilent.x * vsilent.x + vsilent.y * vsilent.y);
    Vector ang;
    VectorAngles(vsilent, ang);
    float yaw = DEG2RAD(ang.y - pCommand->viewangles.y);
    return { cos(yaw) * 450, -sin(yaw) * 450 };
}


struct JumpLog {
    Vector position;
    bool jumped;
};
JumpLog lastJump;

void MoveTo(const Vector& vector, CBaseEntity* pLocal, CUserCmd* pCommand, float followDistance, CBaseEntity* pEntity)
{
    Vector currentPosition = pLocal->GetAbsOrigin();
    float distance = currentPosition.DistTo(vector);

    if (distance <= followDistance) {
        // Stop moving if within the follow distance
        pCommand->forwardmove = 0;
        pCommand->sidemove = 0;
        return;
    }

    auto result = ComputeMove(currentPosition, vector, pCommand);
    pCommand->forwardmove = result.first;
    pCommand->sidemove = result.second;
    if ((pEntity->GetFlags() & FL_ONGROUND) && lastJump.jumped)
    {
        lastJump.position = pEntity->GetAbsOrigin();
        lastJump.jumped = false;
    }
    if (!(pEntity->GetFlags() & FL_ONGROUND) && !lastJump.jumped) {
        lastJump.position = pEntity->GetAbsOrigin();
        lastJump.jumped = true;
        std::string logjump = "echo Logged jump position: " + std::to_string(lastJump.position.x) + ", " + std::to_string(lastJump.position.y) + ", " + std::to_string(lastJump.position.z) + ".";
        gInts.Engine->ClientCmd_Unrestricted(logjump.c_str());
    }
    if (lastJump.jumped && currentPosition.DistTo(lastJump.position) < 50.0f) {
        pCommand->buttons |= IN_JUMP;
        lastJump.jumped = false;
        std::string logjump = "echo Jumped at logged position: " + std::to_string(lastJump.position.x) + ", " + std::to_string(lastJump.position.y) + ", " + std::to_string(lastJump.position.z) + ".";
        gInts.Engine->ClientCmd_Unrestricted(logjump.c_str());
    }
}

void CFollow::Run(CBaseEntity* pLocal, CUserCmd* pCommand)
{
    for (int i = 1; i <= gInts.Engine->GetMaxClients(); i++)
    {
        if (i == me)
            continue;

        CBaseEntity* pEntity = GetBaseEntity(i);

        if (!gCvars.followbot_enabled)
            return;

        if (!pEntity)
            continue;

        if (pEntity->IsDormant())
            continue;

        if (pEntity->GetLifeState() == LIFE_DEAD)
            continue;

        //if (pEntity->GetTeamNum() != pLocal->GetTeamNum())
        //    continue; // the entity that we're following MUST be in OUR team. Who the fuck followbots a enemy? LOL

        float followDistance = gCvars.followbot_distance; // Desired follow distance
        if (pLocal->GetVecOrigin().DistTo(pEntity->GetVecOrigin()) > 3400.0F)
            continue;

        if (pLocal->GetVecOrigin().DistTo(pEntity->GetVecOrigin()) >
            (float)900)
            continue;
        if (!Util->IsVisible(pLocal, pEntity, pLocal->GetEyePosition(), pEntity->GetVecOrigin()))
            continue;


        MoveTo(pEntity->GetVecOrigin(), pLocal, pCommand, followDistance, pEntity);
    }
}
*/
extern CFollow gFollow;