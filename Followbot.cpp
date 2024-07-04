#include "SDK.h"
#include "CDrawManager.h"
#include "Vector.h"
#include "Followbot.h"
#include <vector>
#include <utility>
#include <fstream>
#include <iostream>
#include <limits>
#include "Util.h"
#include "Aimbot.h"

CNav gNav;

std::pair<float, float> ComputeMove(const Vector& a, const Vector& b, CUserCmd* pCommand) {
    Vector diff = (b - a);
    if (diff.Length() == 0)
        return { 0, 0 };
    const float x = diff.x;
    const float y = diff.y;
    Vector vsilent(x, y, 0);
    float speed = sqrt(vsilent.x * vsilent.x + vsilent.y * vsilent.y);
    Vector ang;
    VectorAngles(vsilent, ang);
    float yaw = DEG2RAD(ang.y - pCommand->viewangles.y);
    return { cos(yaw) * 450, -sin(yaw) * 450 };
}

void CNav::StartRecording() {
    nodes.clear();
    isRecording = true;
}

void CNav::StopRecording(const char* filename) {
    if (!isRecording) return;

    isRecording = false;
    std::ofstream file(filename);
    if (!file.is_open()) {
        //std::cerr << "Failed to open file for writing: " << filename << std::endl;
        return;
    }

    for (const auto& node : nodes) {
        file << node.position.x << " " << node.position.y << " " << node.position.z << " " << node.jumped << std::endl;
    }

    file.close();
    //std::cout << "Nodes saved to file: " << filename << std::endl;
    Util->LogToParty("[NavBOT] Nodes saved to file.", true);
}

void CNav::Record(CBaseEntity* pLocal, CUserCmd* pCmd) {
    if (!isRecording) return;

    Vector currentPosition = pLocal->GetAbsOrigin();
    bool jumped = (pCmd->buttons & IN_JUMP);

    // Log current position and whether the player jumped
    nodes.push_back({ currentPosition, jumped });
    //printf("Recorded node: %f, %f, %f, jumped: %d\n", currentPosition.x, currentPosition.y, currentPosition.z, jumped);
}

void CNav::Replay(CBaseEntity* pLocal, CUserCmd* pCommand) {
    if (nodes.empty())
        return;

    float flCurTime = gInts.Engine->Time();
    Vector currentPosition = pLocal->GetAbsOrigin();
    float distanceToLastPos = currentPosition.DistTo(lastPosition);

    // Check if the player is dead
    if (pLocal->GetLifeState() & LIFE_DEAD) {
        currentNode = 0;
        isReversing = false;
        lastProgressTime = flCurTime;
        lastPosition = currentPosition;
        printf("Player dead, restarting from the beginning\n");
        return;
    }

    // Check if we are stuck (not making progress)
    if (distanceToLastPos < 5.0f && (flCurTime - lastProgressTime) > 30.0f && currentNode != 0) 
    {
        // Find the closest node to continue from
        size_t closestNode = currentNode; // Start searching from current node
        float closestDistance = FLT_MAX;

        for (size_t i = currentNode; i < nodes.size(); ++i) {
            float distance = currentPosition.DistTo(nodes[i].position);
            if (distance < closestDistance) {
                closestNode = i;
                closestDistance = distance;
            }
        }

        // If no closer node found after current, search from beginning to current
        if (closestNode == currentNode) {
            closestDistance = FLT_MAX;
            for (size_t i = 0; i < currentNode; ++i) {
                float distance = currentPosition.DistTo(nodes[i].position);
                if (distance < closestDistance) {
                    closestNode = i;
                    closestDistance = distance;
                }
            }
        }

        currentNode = closestNode;
        lastProgressTime = flCurTime;
        lastPosition = currentPosition;
        printf("Bot stuck, moving to closest node: %zu\n", closestNode);
    }

    // Update progress time and position if making progress
    if (distanceToLastPos > 5.0f) {
        lastProgressTime = flCurTime;
        lastPosition = currentPosition;
    }

    if (currentNode >= nodes.size()) {
        currentNode = 0; // Reset to the beginning if exceeded
        isReversing = !isReversing; // Toggle reversing state
        if (isReversing) {
            std::reverse(nodes.begin(), nodes.end()); // Reverse nodes for reverse traversal
        }
    }

    Node& node = nodes[currentNode];
    float distance = currentPosition.DistTo(node.position);

    if (distance < 50.0f) {
        if (node.jumped) {
            pCommand->buttons |= IN_JUMP;
            printf("Jumped at node: %f, %f, %f\n", node.position.x, node.position.y, node.position.z);
        }

        currentNode++;
    }
    else if (currentNode > 0 && currentPosition.DistTo(nodes[currentNode - 1].position) > distance) {
        // If interrupted, find the closest node instead of restarting from the beginning
        size_t closestNode = currentNode; // Start searching from current node
        float closestDistance = FLT_MAX;

        for (size_t i = currentNode; i < nodes.size(); ++i) {
            float distance = currentPosition.DistTo(nodes[i].position);
            if (distance < closestDistance) {
                closestNode = i;
                closestDistance = distance;
            }
        }

        // If no closer node found after current, search from beginning to current
        if (closestNode == currentNode) {
            closestDistance = FLT_MAX;
            for (size_t i = 0; i < currentNode; ++i) {
                float distance = currentPosition.DistTo(nodes[i].position);
                if (distance < closestDistance) {
                    closestNode = i;
                    closestDistance = distance;
                }
            }
        }

        currentNode = closestNode;
        isReversing = false;
    }
    else if (currentNode == 0 && currentPosition.DistTo(nodes.front().position) < 50.0f) {
        // If very close to the first node, continue from there
        currentNode = 0;
    }
    else {
        auto result = ComputeMove(currentPosition, node.position, pCommand);
        pCommand->forwardmove = result.first;
        pCommand->sidemove = result.second;
        
    }
}




bool CNav::LoadFromFile(const char* filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {

        //std::cerr << "Failed to open file for reading: " << filename << std::endl;
        return false;
    }

    nodes.clear();
    Node node;
    while (file >> node.position.x >> node.position.y >> node.position.z >> node.jumped) {
        nodes.push_back(node);
    }

    file.close();
    //std::cout << "Nodes loaded from file: " << filename << std::endl;
    return true;
}

std::string ExtractLevelName(const char* fullPath) {
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

const char* uh(CBaseEntity* pLocal) {
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

void CNav::Run(CBaseEntity* pLocal, CUserCmd* pCommand) {
    if (!pLocal->GetTeamNum())
        return; // make sure we have a team before using teamname.
    std::string teamname = uh(pLocal);
    const char* previousMap = gInts.Engine->GetLevelName();
    std::string filename = ExtractLevelName(previousMap) + "_" + std::string(teamname) + ".txt";
    static bool loadedFromFile = false;
    static bool wasRecording = false;
    static int previousTeamNum = pLocal->GetTeamNum();

    // check this or else our navbot will just use the wrong files
    if (pLocal->GetTeamNum() != previousTeamNum || gInts.Engine->GetLevelName() != previousMap) {
        loadedFromFile = false;
        previousTeamNum = pLocal->GetTeamNum();
        previousMap = gInts.Engine->GetLevelName();

        // reload the file
        if (gCvars.misc_replay) {
            if (!gNav.LoadFromFile(filename.c_str())) {
                //printf("Failed to load nodes from file!\n");
                return;
            }
            loadedFromFile = true;
        }
    }

    if (gCvars.misc_record) {
        if (!wasRecording) {
            gNav.StartRecording();
            wasRecording = true;
        }
        gNav.Record(pLocal, pCommand);
        loadedFromFile = false; // Reset loaded flag when recording
    }
    else if (wasRecording) {
        gNav.StopRecording(filename.c_str());
        wasRecording = false;
    }

    if (gCvars.misc_replay) {
        if (!loadedFromFile) {
            if (!gNav.LoadFromFile(filename.c_str())) {
                //printf("Failed to load nodes from file!\n");
                return;
            }
            loadedFromFile = true;
        }

        gNav.Replay(pLocal, pCommand);
    }
}