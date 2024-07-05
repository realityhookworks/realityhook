#include "SDK.h"
#include "Client.h"
#include "Panels.h"
#include "DUM.h"
#include <cstdio>
#include <string>
#include "Listener.h"
#include "StartUp.h"
COffsets gOffsets;
CGlobalVariables gCvars;
CInterfaces gInts;

CreateInterface_t EngineFactory = NULL;
CreateInterface_t ClientFactory = NULL;
CreateInterface_t VGUIFactory = NULL;
CreateInterface_t VGUI2Factory = NULL;

DWORD WINAPI dwMainThread( LPVOID lpArguments )
{
	//"mss32.dll" being one of the last modules to be loaded
	//So wait for that before proceeding, after it's up everything else should be too
	//Allows us to correctly use autoinject and just start the game.
	while (!GetModuleHandleW(L"mss32.dll") || !GetModuleHandleW(L"ntdll.dll") || !GetModuleHandleW(L"stdshader_dx9.dll") || !GetModuleHandleW(L"materialsystem.dll"))
	{
		Sleep(2000);
	}
	if (gInts.Client == NULL)
	{
		
		
		VMTBaseManager* clientHook = new VMTBaseManager();
		VMTBaseManager* clientModeHook = new VMTBaseManager();
		VMTBaseManager* panelHook = new VMTBaseManager();

		ClientFactory = ( CreateInterfaceFn ) GetProcAddress( gSignatures.GetModuleHandleSafe( "client.dll" ), "CreateInterface" );
		EngineFactory = (CreateInterfaceFn)GetProcAddress(gSignatures.GetModuleHandleSafe("engine.dll"), "CreateInterface");
		VGUIFactory = (CreateInterfaceFn)GetProcAddress(gSignatures.GetModuleHandleSafe("vguimatsurface.dll"), "CreateInterface");

		gInts.Client = ( CHLClient* )ClientFactory( "VClient017", NULL);
		gInts.EntList = ( CEntList* ) ClientFactory( "VClientEntityList003", NULL );
		gInts.Engine = ( EngineClient* ) EngineFactory( "VEngineClient013", NULL );
		gInts.Surface = ( ISurface* ) VGUIFactory( "VGUI_Surface030", NULL );
		gInts.EngineTrace = ( IEngineTrace* ) EngineFactory( "EngineTraceClient003", NULL );
		gInts.ModelInfo = ( IVModelInfo* ) EngineFactory( "VModelInfoClient006", NULL );
		gInts.EventManager = (IGameEventManager2*)EngineFactory("GAMEEVENTSMANAGER002", NULL);

		gListener.Init();

		gInts.Globals = *reinterpret_cast<CGlobals**>(gSignatures.GetEngineSignature("A1 ? ? ? ? 8B 11 68") + 0x8);//g_Pattern.Find(ENGINE, _(L"A1 ? ? ? ? 8B 11 68")) + 0x8);
		XASSERT(gInts.Client);
		XASSERT(gInts.EntList);
		XASSERT(gInts.Engine);
		XASSERT(gInts.Surface);
		XASSERT(gInts.EngineTrace);
		XASSERT(gInts.ModelInfo);

		gInts.PartyClient = reinterpret_cast<CTFPartyClient * (__cdecl*)()>(gSignatures.GetClientSignature("A1 ? ? ? ? C3 CC CC CC CC CC CC CC CC CC CC 55 8B EC 83 EC ? 53"))();
		/* TFGCClientSystem = *S::TFGCClientSystem_Interface.As<CTFGCClientSystem**>(); */
		// i think this is how you do it lol 
		gInts.GClientPartySystem = *reinterpret_cast<CTFGCClientSystem**>(gSignatures.GetClientSignature("B9 ? ? ? ? 50 E8 ? ? ? ? 8B 5D") + 1);

		//Setup the Panel hook so we can draw.
		if( !gInts.Panels )
		{
			VGUI2Factory = (CreateInterfaceFn)GetProcAddress(gSignatures.GetModuleHandleSafe("vgui2.dll"), "CreateInterface");
			gInts.Panels = ( IPanel* ) VGUI2Factory( "VGUI_Panel009", NULL );
			XASSERT( gInts.Panels );

			if( gInts.Panels )
			{
				panelHook->Init(gInts.Panels);
				panelHook->HookMethod(&Hooked_PaintTraverse, gOffsets.iPaintTraverseOffset);
				panelHook->Rehook();
			}
		}

		DWORD dwClientModeAddress = gSignatures.GetClientSignature("8B 0D ? ? ? ? 8B 02 D9 05");
		XASSERT(dwClientModeAddress);
		gInts.ClientMode = **(ClientModeShared***)(dwClientModeAddress + 2);
		LOGDEBUG("g_pClientModeShared_ptr client.dll+0x%X", (DWORD)gInts.ClientMode - dwClientBase);
		
		
		clientHook->Init(gInts.Client);
		//clientHook->HookMethod(&Hooked_KeyEvent, gOffsets.iKeyEventOffset);
		clientHook->HookMethod(&Hooked_FrameStage, gOffsets.iFrameStageOffset);
		/* MH_CreateHook(Utils::GetVirtual(gInts.Client, 36), &dispatch_user_message, reinterpret_cast<LPVOID*>(&oDispatch)); */
		clientHook->HookMethod(&dispatch_user_message, 36);
		clientHook->Rehook();

		clientModeHook->Init(gInts.ClientMode);
		clientModeHook->HookMethod(&Hooked_CreateMove, gOffsets.iCreateMoveOffset); //ClientMode create move is called inside of CHLClient::CreateMove, and thus no need for hooking WriteUserCmdDelta.
		clientModeHook->Rehook();
	}
	return 0; //The thread has been completed, and we do not need to call anything once we're done. The call to Hooked_PaintTraverse is now our main thread.
}

BOOL APIENTRY DllMain(HMODULE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	if(dwReason == DLL_PROCESS_ATTACH)
	{
		Log::Init(hInstance);
		CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)dwMainThread, NULL, 0, NULL ); 
	}
	return true;
}