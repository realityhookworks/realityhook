#include "SDK.h"
#include "Client.h"
#include "Util.h"
#include "Aimbot.h"
#include "Followbot.h"
#include "ChatSpam.h"
#include "Misc.h"
//============================================================================================
bool __fastcall Hooked_CreateMove(PVOID ClientMode, int edx, float input_sample_frametime, CUserCmd* pCommand)
{
	VMTManager& hook = VMTManager::GetHook(ClientMode); //Get a pointer to the instance of your VMTManager with the function GetHook.
	bool bReturn = hook.GetMethod<bool(__thiscall*)(PVOID, float, CUserCmd*)>(gOffsets.iCreateMoveOffset)(ClientMode, input_sample_frametime, pCommand); //Call the original.
	try
	{
		if (!pCommand->command_number)
			return false;

		CBaseEntity* pLocal = GetBaseEntity(me); 

		if (!pLocal)
			return bReturn;

		
		if (pLocal->GetTeamNum() || pLocal->GetClassNum())
		{
			if (gCvars.misc_micspam)
			{
				gInts.Engine->ClientCmd_Unrestricted("+voicerecord");
				gInts.Engine->ClientCmd_Unrestricted("voice_loopback 1");
			}
		}
		else
		{
			if (gCvars.misc_micspam)
			{
				gInts.Engine->ClientCmd_Unrestricted("-voicerecord");
			}
		}
		gSpam.Run();
		gMisc.Run(pLocal, pCommand);
		gAim.Run(pLocal, pCommand);
		gNav.Run(pLocal, pCommand);
	}
	catch(...)
	{
		Log::Fatal("Failed Hooked_CreateMove");
	}
	return false/*bReturn*/;
}
//============================================================================================
//============================================================================================
void __fastcall Hooked_FrameStage(PVOID CHLClient, int edx, ClientFrameStage_t stage) {
	VMTManager& hook = VMTManager::GetHook(CHLClient);

	bool frame_modify = true;

	switch (stage) {
	case FRAME_NET_UPDATE_POSTDATAUPDATE_START:
		//gAntiAntiAim.PostDataUpdateStart(); // this is acting as a simple anti anti aim, do not expect too much out of this.
		break;
	default:
		frame_modify = false;
		break;
	}

	hook.GetMethod<void(__thiscall*)(PVOID, ClientFrameStage_t)>(gOffsets.iFrameStageOffset)(CHLClient, stage); // call the original but don't return, since this is a void.
}
//============================================================================================