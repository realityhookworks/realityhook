#include "SDK.h"

class CGlobalVariables
{
public:
	int iAimbotIndex = -1;
	
	float settings_switch;
	float iMenu_Pos_X = 300;
	float iMenu_Pos_Y = 75;
	float szsubstest;

	float aimbot_switch;
	float aimbot_silent;
	float aimbot_active;
	float aimbot_key;
	float aimbot_hitscan;
	float aimbot_hitbox;
	float aimbot_autoshoot;
	float aimbot_zoomedonly;
	float aimbot_autoscope;
	float aimbot_fov;

	float misc_switch;
	float misc_bunnyhop;
	float misc_autostrafe;
	float misc_noisemaker_spam;
	float misc_autoqueue;
	float misc_record;
	float misc_replay;
	float misc_micspam;
	float misc_chatspam;
	float misc_namestealer = 1.0f;

	/* anti-aim */
	float antiaim_switch;
	float antiaim_enable;
	float antiaim_pitch;
	float antiaim_yaw;
	float antiaim_spinspeed;
	/* thirdperson */
	float misc_thirdperson;
	bool isThirdPerson = false; /* dont add this to the menu, nor the config */
	/* | never add these to the config manager (if you do, youre a retard)*/
	/* V*/
	float config_save;
	float config_load;

	float playerlist_switch;
	float PlayerMode[64] = {
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	};
};

class COffsets
{
public:
	int iKeyEventOffset = 20, iCreateMoveOffset = 21, iPaintTraverseOffset = 41, iFrameStageOffset = 35;
};