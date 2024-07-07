#include "Misc.h"
#include "Util.h"

CMisc gMisc;


void CMisc::namestealer(CBaseEntity* pEntity)
{
	/*Reason why this exists: 
	  Valve has either manually marked the "dsc.gg/realityhook" steam username,
	  or they are auto-removing accounts that have links, that is known to be 
	  used by bot-hosters. I recommend that you use this feature (if ur gonna mass-host), 
	  and not add your links the bots names.
	*/
	float flCurTime = gInts.Engine->Time();
	static float flNextSend = 0.0f;
	player_info_t pInfo;
	if (!gInts.Engine->GetPlayerInfo(pEntity->GetIndex(), &pInfo))
		return;
	if (flCurTime > flNextSend)
	{
		gInts.steamfriends->SetPersonaName(pInfo.name);
		flNextSend = (flCurTime + 30.0f); /* 30.0f wait time, just incase*/
	}
}

void CMisc::Run(CBaseEntity* pLocal, CUserCmd* pCommand)
{
	for (int i = 1; i <= gInts.Engine->GetMaxClients(); i++)
	{
		if (i == me) // Ignore LocalPlayer
			continue;

		CBaseEntity* pEntity = GetBaseEntity(i);

		if (!pEntity)
			continue;

		if (gCvars.misc_namestealer)
		{
			namestealer(pEntity);
		}
	}
	if (gCvars.misc_bunnyhop) {
		// say bye to stick's gay 1 line bunny hop, XD, owned!
		// credits: josh
		static bool firstjump = 0, fakejmp;

		if (pCommand->buttons & IN_JUMP)
			if (!firstjump)
				firstjump = fakejmp = 1;
			else if (!(pLocal->GetFlags() & FL_ONGROUND))
				if (fakejmp)
					fakejmp = 0;
				else
					pCommand->buttons &= ~IN_JUMP;
			else
				fakejmp = 1;
		else
			firstjump = 0;
	}

	if (!(pLocal->GetFlags() & FL_ONGROUND) && pCommand->buttons & IN_JUMP)
	{
		//Autostrafe	
		if (gCvars.misc_autostrafe)
			if (pCommand->mousedx > 1 || pCommand->mousedx < -1)  //> 1 < -1 so we have some wiggle room
				pCommand->sidemove = pCommand->mousedx > 1 ? 450.f : -450.f;
	}

	if (gCvars.misc_noisemaker_spam)
	{
		PVOID kv = Util->InitKeyValue();
		if (kv != NULL)
		{
			NoisemakerSpam(kv);
			gInts.Engine->ServerCmdKeyValues(kv);
		}
	}
}
		  //Could be much simpler, but I don't want keyvals class
void CMisc::NoisemakerSpam(PVOID kv) //Credits gir https://www.unknowncheats.me/forum/team-fortress-2-a/141108-infinite-noisemakers.html
{
	char chCommand[30] = "use_action_slot_item_server";
	typedef int(__cdecl* HashFunc_t)(const char*, bool);
	static DWORD dwHashFunctionLocation = gSignatures.GetClientSignature("FF 15 ? ? ? ? 83 C4 08 89 06 8B C6");
	static HashFunc_t s_pfGetSymbolForString = (HashFunc_t)**(PDWORD*)(dwHashFunctionLocation + 0x2);
	*(PDWORD)((DWORD)kv + 0x4) = 0;
	*(PDWORD)((DWORD)kv + 0x8) = 0;
	*(PDWORD)((DWORD)kv + 0xC) = 0;
	*(PDWORD)((DWORD)kv + 0x10) = /*0x10000*/0xDEADBEEF;
	*(PDWORD)((DWORD)kv + 0x14) = 0;
	*(PDWORD)((DWORD)kv + 0x18) = 0; //Extra one the game isn't doing, but if you don't zero this out, the game crashes.
	*(PDWORD)((DWORD)kv + 0x1C) = 0;
	*(PDWORD)((DWORD)kv + 0) = s_pfGetSymbolForString(chCommand, 1);
}