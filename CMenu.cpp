#include "CMenu.h"
#include "SDK.h"
#include "CDrawManager.h"
#include "Util.h"
#include "ConfigManager.h"
CCheatMenu gCheatMenu;

int ScreenH;
int ScreenW;

char* szKeyNames[] = {
	"None", "Mouse 1", "Mouse 2", "Mouse 3", "Mouse 4", "Mouse 5", "Shift", "Alt", "F",
};

char* szHealthModes[] = {
	"OFF", "Text", "Bar", "Both",
};

char* szBoneModes[] = {
	"OFF", "White", "Health", "Team",
};

char* szHitboxes[] =
{
	"Auto", "Head", "Pelvis", "Lower Torse", "Lower Mid. Torse", "Upper Mid. Torse", "Upper Torse",
	"Left Upper Arm", "Left Lower Arm", "Left Hand", "Right Upper Arm",
	"Right Lower Arm", "Right Hand", "Left Hip", "Left Knee", "Left Foot",
	"Right Hip", "Right Knee", "Right Foot",
};

char* pitch[] = {
	"None", "Fake UP", "UP", "Fake Down", "Down"
};
char* yaw[] = {
	"None", "Right", "Left", "Back", "Emotion", "Random", "Fake Sideways Right", "Fake Sideways Left", "Fake Right", "Half Backwards Left", "Half Backwards Right", "Fake Left", "Spin"
};

int CCheatMenu::AddItem(int nIndex, char szTitle[30], float* value, float flMin, float flMax, float flStep, bool isClassSwitch)
{
	strcpy(pMenu[nIndex].szTitle, szTitle);
	pMenu[nIndex].value = value;
	pMenu[nIndex].flMin = flMin;
	pMenu[nIndex].flMax = flMax;
	pMenu[nIndex].flStep = flStep;
	pMenu[nIndex].isClassSwitch = isClassSwitch;
	return (nIndex + 1);
}

void CCheatMenu::Render(void)
{
	gInts.Engine->GetScreenSize(ScreenW, ScreenH);

	int i = 0;

	i = AddItem(i, "Aimbot", &gCvars.aimbot_switch, 0, 1, 1, true);
	if (gCvars.aimbot_switch)
	{
		i = AddItem(i, " - Enabled", &gCvars.aimbot_active, 0, 1, 1, false);
		i = AddItem(i, " - Silent", &gCvars.aimbot_silent, 0, 1, 1, false);
		i = AddItem(i, " - Fov", &gCvars.aimbot_fov, 0, 280, 1, false);
		i = AddItem(i, " - Hitbox", &gCvars.aimbot_hitbox, 0, 18, 1, false);
		i = AddItem(i, " - Autoshoot", &gCvars.aimbot_autoshoot, 0, 1, 1, false);
		i = AddItem(i, " - AutoScope", &gCvars.aimbot_autoscope, 0, 1, 1, false);
		i = AddItem(i, " - ZoomedOnly", &gCvars.aimbot_zoomedonly, 0, 1, 1, false);
	}
	i = AddItem(i, "Player List", &gCvars.playerlist_switch, 0, 1, 1, true);
	for (int p = 1; p <= gInts.Engine->GetMaxClients(); p++)
	{
		if (p == me)
			continue;

		CBaseEntity *pPlayer = GetBaseEntity(p);

		if (pPlayer == NULL)
			continue;

		player_info_t pInfo;
		gInts.Engine->GetPlayerInfo(p, &pInfo);

		char szString[256];
		sprintf(szString, " - %s", pInfo.name);

		if (gCvars.playerlist_switch)
			i = AddItem(i, szString, &gCvars.PlayerMode[pPlayer->GetIndex()], 0, 2, 1, false);
	}

	i = AddItem(i, "Settings", &gCvars.settings_switch, 0, 1, 1, true);
	if (gCvars.settings_switch)
	{
		i = AddItem(i, " - Menu Postion X", &gCvars.iMenu_Pos_X, 0, ScreenW, 25, false);
		i = AddItem(i, " - Menu Postion Y", &gCvars.iMenu_Pos_Y, 0, ScreenH, 25, false);
		i = AddItem(i, " - Save Config", &gCvars.config_save, 0, 1, 1, false);
		i = AddItem(i, " - Load Config", &gCvars.config_load, 0, 1, 1, false); // it already auto-loads config on inject but whatever
		if (gCvars.config_save)
		{
			gConfig.Save();
		}
		if (gCvars.config_load)
		{
			gConfig.Load();
		}
	}
	i = AddItem(i, "Anti-Aim", &gCvars.antiaim_switch, 0, 1, 1, true);
	if (gCvars.antiaim_switch)
	{
		i = AddItem(i, "- Enable", &gCvars.antiaim_enable, 0, 1, 1, false);
		i = AddItem(i, "- Pitch", &gCvars.antiaim_pitch, 0, 5, 1, false);
		i = AddItem(i, "- Yaw", &gCvars.antiaim_yaw, 0, 13, 1, false);
	}

	i = AddItem(i, "Misc", &gCvars.misc_switch, 0, 1, 1, true);
	if (gCvars.misc_switch)
	{
		i = AddItem(i, " - Bunnyhop", &gCvars.misc_bunnyhop, 0, 1, 1, false);
		i = AddItem(i, " - Autostrafe", &gCvars.misc_autostrafe, 0, 1, 1, false);
		i = AddItem(i, " - Noisemaker Spam", &gCvars.misc_noisemaker_spam, 0, 1, 1, false);
		i = AddItem(i, " - AutoQueue & AutoRQ", &gCvars.misc_autoqueue, 0, 1, 1, false);
		i = AddItem(i, " - Thirdperson (KEY B)", &gCvars.misc_thirdperson, 0, 1, 1, false);
		i = AddItem(i, " - Micspam", &gCvars.misc_micspam, 0, 1, 1, false);
		i = AddItem(i, " - ChatSpam", &gCvars.misc_chatspam, 0, 1, 1, false);
		i = AddItem(i, " - Record", &gCvars.misc_record, 0, 1, 1, false);
		i = AddItem(i, " - Replay", &gCvars.misc_replay, 0, 1, 1, false);
	}

	iMenuItems = i;
}
void CCheatMenu::HandleControls(void)
{
	float flCurTime = gInts.Engine->Time();
	static float flNextSend = 0.0f;
	static float thesleeptime = 0.1f;
	if (GetAsyncKeyState(VK_INSERT) && flCurTime > flNextSend) //insert
	{
		flNextSend = (flCurTime + thesleeptime);
		gCheatMenu.bMenuActive = !gCheatMenu.bMenuActive;
	}

	if (gCheatMenu.bMenuActive)
	{

		if (GetAsyncKeyState(VK_UP) && flCurTime > flNextSend) // Up
		{
			flNextSend = (flCurTime + thesleeptime);
			if (gCheatMenu.iMenuIndex > 0) gCheatMenu.iMenuIndex--;
			else gCheatMenu.iMenuIndex = gCheatMenu.iMenuItems - 1;
			//return 0;
		}
		if (GetAsyncKeyState(VK_DOWN) && flCurTime > flNextSend) // Down
		{
			flNextSend = (flCurTime + thesleeptime);
			if (gCheatMenu.iMenuIndex < gCheatMenu.iMenuItems - 1) gCheatMenu.iMenuIndex++;
			else gCheatMenu.iMenuIndex = 0;
			//return 0;
		}
		if (GetAsyncKeyState(VK_LEFT) && flCurTime > flNextSend) // Left
		{
			if (gCheatMenu.pMenu[gCheatMenu.iMenuIndex].value && flCurTime > flNextSend)
			{
				flNextSend = (flCurTime + thesleeptime);
				gCheatMenu.pMenu[gCheatMenu.iMenuIndex].value[0] -= gCheatMenu.pMenu[gCheatMenu.iMenuIndex].flStep;
				if (gCheatMenu.pMenu[gCheatMenu.iMenuIndex].value[0] < gCheatMenu.pMenu[gCheatMenu.iMenuIndex].flMin)
					gCheatMenu.pMenu[gCheatMenu.iMenuIndex].value[0] = gCheatMenu.pMenu[gCheatMenu.iMenuIndex].flMax;
			}
		}
		if (GetAsyncKeyState(VK_RIGHT) && flCurTime > flNextSend) // Right
		{
			if (gCheatMenu.pMenu[gCheatMenu.iMenuIndex].value && flCurTime > flNextSend)
			{
				flNextSend = (flCurTime + thesleeptime);
				gCheatMenu.pMenu[gCheatMenu.iMenuIndex].value[0] += gCheatMenu.pMenu[gCheatMenu.iMenuIndex].flStep;
				if (gCheatMenu.pMenu[gCheatMenu.iMenuIndex].value[0] > gCheatMenu.pMenu[gCheatMenu.iMenuIndex].flMax)
					gCheatMenu.pMenu[gCheatMenu.iMenuIndex].value[0] = gCheatMenu.pMenu[gCheatMenu.iMenuIndex].flMin;
			}
		}

	}
	//}
}
void CCheatMenu::DrawMenu(void)
{
	int x = gCvars.iMenu_Pos_X,
		xx = x + 150,
		y = gCvars.iMenu_Pos_Y,
		w = 200,
		h = 14;


	Color clrColor = Color(255, 20, 20, 255);

	gDrawManager.DrawRect(x, y - (h + 4), w, iMenuItems * h + 21, Color(20, 20, 20, 128));
	gDrawManager.OutlineRect(x, y - (h + 4), w, (h + 4), clrColor);

	gDrawManager.DrawRect(x + 2, y - (h + 4), w - 4, (h + 4), clrColor);
	gDrawManager.OutlineRect(x - 1, y - (h + 4) - 1, w + 2, (h + 4), Color(0, 0, 0, 255)); // test
	gDrawManager.OutlineRect(x + 1, y - (h + 4) + 1, w - 2, (h + 4), Color(0, 0, 0, 255)); // test

	gDrawManager.OutlineRect(x, y - (h + 4), w, iMenuItems * h + 21, clrColor);

	gDrawManager.OutlineRect(x - 1, (y - (h + 4)) - 1, w + 2, (iMenuItems * h + 21) + 2, Color(0, 0, 0, 255));
	gDrawManager.OutlineRect(x + 1, (y - (h + 4)) + 1, w - 2, (iMenuItems * h + 21) - 2, Color(0, 0, 0, 255));

	gDrawManager.DrawString(x + 4, y - 16, clrColor, "Polly.xyz");

	for (int i = 0; i < iMenuItems; i++)
	{
		if (i != iMenuIndex)
		{
			if (pMenu[i].isClassSwitch)
			{
				if (!(pMenu[i].value[0]))
				{
					gDrawManager.DrawString(x + 2, y + (h * i), clrColor, "[+] %s", pMenu[i].szTitle);
				}
				else if (pMenu[i].value[0])
				{
					gDrawManager.DrawString(x + 2, y + (h * i), clrColor, "[-] %s", pMenu[i].szTitle);
				}
			}
			else
			{
				gDrawManager.DrawString(x + 4, y + (h * i), Color::White(), pMenu[i].szTitle);

				if (!strcmp(pMenu[i].szTitle, " - Health"))
				{
					gDrawManager.DrawString(xx, y + (h * i), pMenu[i].value[0] ? Color::White() : Color(105, 105, 105, 255), "%s", szHealthModes[(int)pMenu[i].value[0]]);
				}

				else if (!strcmp(pMenu[i].szTitle, " - Bones"))
				{
					gDrawManager.DrawString(xx, y + (h * i), pMenu[i].value[0] ? Color::White() : Color(105, 105, 105, 255), "%s", szBoneModes[(int)pMenu[i].value[0]]);
				}

				else if (!strcmp(pMenu[i].szTitle, " - Pitch"))
				{
					gDrawManager.DrawString(xx, y + (h * i), pMenu[i].value[0] ? Color::White() : Color(105, 105, 105, 255), "%s", pitch[(int)pMenu[i].value[0]]);
				}
				else if (!strcmp(pMenu[i].szTitle, " - Yaw"))
				{
					gDrawManager.DrawString(xx, y + (h * i), pMenu[i].value[0] ? Color::White() : Color(105, 105, 105, 255), "%s", yaw[(int)pMenu[i].value[0]]);
				}

				else if (pMenu[i].flMax == 18)
				{
					gDrawManager.DrawString(xx, y + (h * i), Color::White(), "%s", szHitboxes[(int)pMenu[i].value[0]]);
				}

				else if (pMenu[i].flMax == 8)
				{
					gDrawManager.DrawString(xx, y + (h * i), Color::White(), "%s", szKeyNames[(int)pMenu[i].value[0]]);
				}

				else if (pMenu[i].flMax == 2)
				{
					gDrawManager.DrawString(xx, y + (h * i), Color::White(), !pMenu[i].value[0] ? "Ignore" : pMenu[i].value[0] == 1 ? "Normal" : "Rage");
				}

				else if (pMenu[i].flMax == 1)
				{
					gDrawManager.DrawString(xx, y + (h * i), pMenu[i].value[0] ? Color::White() : Color(105, 105, 105, 255), pMenu[i].value[0] ? "ON" : "OFF");
				}

				else if (pMenu[i].value[0] >= 1 && pMenu[i].flMax > 1)
				{
					gDrawManager.DrawString(xx, y + (h * i), pMenu[i].value[0] >= 1 ? Color::White() : Color(105, 105, 105, 255), "%0.0f", pMenu[i].value[0]);
				}
			}
		}
		else
		{
			gDrawManager.DrawRect(x + 1, y + (h * i), w - 2, h, Color(255, 255, 255, 80));

			if (pMenu[i].isClassSwitch)
			{
				if (!(pMenu[i].value[0]))
				{
					gDrawManager.DrawString(x + 2, y + (h * i), clrColor, "[+] %s", pMenu[i].szTitle);
				}
				else if (pMenu[i].value[0])
				{
					gDrawManager.DrawString(x + 2, y + (h * i), clrColor, "[-] %s", pMenu[i].szTitle);
				}
			}
			else
			{
				gDrawManager.DrawString(x + 4, y + (h * i), clrColor, pMenu[i].szTitle);

				if (!strcmp(pMenu[i].szTitle, " - Health"))
				{
					gDrawManager.DrawString(xx, y + (h * i), clrColor, "%s", szHealthModes[(int)pMenu[i].value[0]]);
				}

				else if (!strcmp(pMenu[i].szTitle, " - Bones"))
				{
					gDrawManager.DrawString(xx, y + (h * i), clrColor, "%s", szBoneModes[(int)pMenu[i].value[0]]);
				}

				else if (pMenu[i].flMax == 18)
				{
					gDrawManager.DrawString(xx, y + (h * i), clrColor, "%s", szHitboxes[(int)pMenu[i].value[0]]);
				}

				else if (pMenu[i].flMax == 8)
				{
					gDrawManager.DrawString(xx, y + (h * i), clrColor, "%s", szKeyNames[(int)pMenu[i].value[0]]);
				}

				else if (pMenu[i].flMax == 2)
				{
					gDrawManager.DrawString(xx, y + (h * i), clrColor, !pMenu[i].value[0] ? "Ignore" : pMenu[i].value[0] == 1 ? "Normal" : "Rage");
				}

				else if (pMenu[i].flMax == 1)
				{
					gDrawManager.DrawString(xx, y + (h * i), clrColor, pMenu[i].value[0] ? "ON" : "OFF");
				}

				else
				{
					gDrawManager.DrawString(xx, y + (h * i), clrColor, "%0.0f", pMenu[i].value[0]);
				}
			}
		}
	}
}