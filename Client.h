#pragma once

#include "SDK.h"

class CGlobalzz {
public:
	bool* sendpacket;
};

bool __fastcall Hooked_CreateMove(PVOID ClientMode, int edx, float input_sample_frametime, CUserCmd* pCommand);
int __fastcall Hooked_KeyEvent(PVOID CHLClient, int edx, int eventcode, int keynum, const char *currentBinding);
void __fastcall Hooked_FrameStage(PVOID CHLClient, int edx, ClientFrameStage_t stage);

extern CGlobalzz g;