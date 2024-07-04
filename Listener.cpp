#include "Listener.h"
#include "Followbot.h"
#include "Util.h"
#define GET_PLAYER_USERID(userid) gInts.EntList->GetClientEntity(gInts.Engine->GetPlayerForUserID(userid))

CListener gListener;
void CListener::Init()
{
	gInts.EventManager->AddListener(this, "player_death", false);
	gInts.EventManager->AddListener(this, "player_hurt", false);
	gInts.EventManager->AddListener(this, "player_disconnect", false);
	gInts.EventManager->AddListener(this, "player_changeclass", false);
	gInts.EventManager->AddListener(this, "vote_cast", false);
	gInts.EventManager->AddListener(this, "game_init", false);
	gInts.EventManager->AddListener(this, "game_start", false);
	gInts.EventManager->AddListener(this, "game_newmap", false);
	wasEventInitialized = true;
}
std::string extract(const char* fullPath) {
    std::string pathString = fullPath;
    size_t lastSlashPos = pathString.find_last_of('/');
    size_t lastDotPos = pathString.find_last_of('.');

    if (lastSlashPos != std::string::npos && lastDotPos != std::string::npos && lastDotPos > lastSlashPos + 1) {
        return pathString.substr(lastSlashPos + 1, lastDotPos - lastSlashPos - 1);
    }
    else {
        // If no valid slash and dot found, return the entire string
        return pathString;
    }
}

const char* nigger(CBaseEntity* pLocal) {
    if (!pLocal->GetTeamNum())
        return "UNKNOWN";
    if (pLocal->GetTeamNum() == RED_TEAM) {
        return "Red";
    }
    if (pLocal->GetTeamNum() == BLU_TEAM) {
        return "Blu";
    }
    return "UNKNOWN";
}
void CListener::FireGameEvent(IGameEvent* event)
{
	if ( (!strcmp(event->GetName(), "game_newmap")))
	{
		/* reload the nav if the map changed lol */
        CBaseEntity* pLocal = GetBaseEntity(me);

        if (!pLocal)
            return;

        std::string teamname = nigger(pLocal);
        const char* map = gInts.Engine->GetLevelName();
        std::string filename = extract(map) + "_" + std::string(teamname) + ".txt";

        gNav.LoadFromFile(filename.c_str());
        Util->LogToParty("[NavBOT] game_newmap event detected! reloading the nav file", true);
	}
}