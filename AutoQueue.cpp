#include "AutoQueue.h"

CQueue gQueue;
#define casual 7
void CQueue::Run()
{
	const bool bInQueueForMatchGroup = gInts.PartyClient->BInQueueForMatchGroup(casual);
	const int nNumMatchInvites = gInts.GClientPartySystem->GetNumMatchInvites();
	const bool bInStandbyQueue = gInts.PartyClient->BInStandbyQueue();
	bool bInGame = (IfCanqueue());
	const bool bHaveLiveMatch = gInts.GClientPartySystem->BHaveLiveMatch();
    if (gCvars.misc_autoqueue)
    {
        if (bInQueueForMatchGroup || nNumMatchInvites || bInStandbyQueue || bInGame || bHaveLiveMatch)
            return;

        if (!bInQueueForMatchGroup && !nNumMatchInvites && !bInStandbyQueue && !bInGame && !bHaveLiveMatch)
        {
            gInts.PartyClient->LoadSavedCasualCriteria();
            gInts.PartyClient->RequestQueueForMatch(casual);
        }
    }
}
/*
    if (playerCount > 5)
        {
            std::string debug = "echo playercount more than 5, returning true, playercount: " + std::to_string(playerCount);
            gInts.Engine->ClientCmd_Unrestricted(debug.c_str());
            return true;
        }
        else
        {
            //gInts.Engine->ClientCmd_Unrestricted("echo playercount less than 5 , returning false");
            std::string debug2 = "echo playercount less than 5, returning false, playercount: " + std::to_string(playerCount);
            gInts.Engine->ClientCmd_Unrestricted(debug2.c_str());
            return false;
        }
    */
bool CQueue::IfCanqueue()
{
    const bool bInGameThing = (gInts.Engine->IsInGame() || !gInts.Engine->IsConnected());
    int playerCount = 0;
    for (int i = 1; i <= gInts.Engine->GetMaxClients(); i++)
    {
        if (i == me)
            continue;

        CBaseEntity* pEntity = GetBaseEntity(i);

        if (!pEntity)
            continue;

        playerCount++;
    }
    
    if (bInGameThing && playerCount > 5)
    {
        // only for debug.
        //std::string debug = "echo playercount more than 5, returning true, playercount: " + std::to_string(playerCount);
        //gInts.Engine->ClientCmd_Unrestricted(debug.c_str());
        return true;
    }
    
    if (!bInGameThing || playerCount < 5)
        //gInts.Engine->ClientCmd_Unrestricted("echo line 64 in autoqueue");
        return false;
}