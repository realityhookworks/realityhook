#include "SDK.h"
#include "Panels.h"
#include "CDrawManager.h"
#include "Util.h"
#include "ConfigManager.h"
#include "StartUp.h"
#include "AutoQueue.h"

CScreenSize gScreenSize;
//===================================================================================
void __fastcall Hooked_PaintTraverse( PVOID pPanels, int edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce )
{
	try
	{
		VMTManager& hook = VMTManager::GetHook(pPanels); 
		hook.GetMethod<void(__thiscall*)(PVOID, unsigned int, bool, bool)>(gOffsets.iPaintTraverseOffset)(pPanels, vguiPanel, forceRepaint, allowForce); //Call the original.

		static unsigned int vguiFocusOverlayPanel;

		if (vguiFocusOverlayPanel == NULL)
		{											//FocusOverlayPanel
			const char* szName = gInts.Panels->GetName(vguiPanel);
			if( szName[0] == 'F' && szName[5] == 'O' &&  szName[12] == 'P' )
			{
				vguiFocusOverlayPanel = vguiPanel;
				Intro();
			}
		}
		
		if (vguiFocusOverlayPanel == vguiPanel )
		{
			int iWidth, iHeight; //Resolution fix, so this can work in Fullscreen
			gInts.Engine->GetScreenSize(iWidth, iHeight);
			if (gScreenSize.iScreenWidth != iWidth || gScreenSize.iScreenHeight != iHeight)
				gInts.Engine->GetScreenSize(gScreenSize.iScreenWidth, gScreenSize.iScreenHeight);

			static bool once = false;
			if (!once)
			{
				gConfig.Load();
				gStart.executeCommandsFromFileList("realitybot_autoexec.txt");
				once = true;
			}
			gQueue.Run();
			gDrawManager.DrawString(5, 5, Color::White(), "realityhook");
			gCheatMenu.HandleControls();
			if (gCheatMenu.bMenuActive)
			{
				gCheatMenu.DrawMenu();
				gCheatMenu.Render();
			}
		}
	}
	catch(...)
	{
		Log::Fatal("Failed PaintTraverse");
	}
}
//===================================================================================
void Intro( void )
{
	try
	{
		gDrawManager.Initialize(); //Initalize the drawing class.

		gNetVars.Initialize();
	}
	catch(...)
	{
		Log::Fatal("Failed Intro");
	}
}