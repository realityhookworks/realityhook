#include "Listener.h"

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
void CListener::FireGameEvent(IGameEvent* event)
{
	if ( (!strcmp(event->GetName(), "game_init")) || (!strcmp(event->GetName(), "game_start")) || (!strcmp(event->GetName(), "game_newmap")))
	{
		
	}
}