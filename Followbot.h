#pragma once
#include "SDK.h"
#include <mutex>
#include <deque>
struct Node {
    Vector position;
    bool jumped;
};
class CNav
{
public:
	
	void Run(CBaseEntity* pLocal, CUserCmd* pCmd);
    void StartRecording();
    void StopRecording(const char* filename);
    void Record(CBaseEntity* pLocal, CUserCmd* pCmd);
    void Replay(CBaseEntity* pLocal, CUserCmd* pCommand);
    bool LoadFromFile(const char* filename);
    std::vector<Node> nodes;
    size_t currentNode = 0;
    bool isRecording = false;
    bool isReversing = false;
    float lastProgressTime = 0.0f;
    Vector lastPosition;
};
extern CNav gNav;