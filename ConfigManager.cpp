#include "SDK.h"
#include "json.h"
#include <fstream>
#include <string>
#include "ConfigManager.h"
CConfig gConfig;

void CConfig::Save()
{
    std::ofstream output("realityhook.json");

    if (output.is_open())
    {
        Json::Value Save;
        /*
        * Aimbot
        */
        Save["gAim"]["active"] = gCvars.aimbot_active;
        Save["gAim"]["silent"] = gCvars.aimbot_silent;
        Save["gAim"]["hitscan"] = gCvars.aimbot_hitscan;
        Save["gAim"]["hitbox"] = gCvars.aimbot_hitbox;
        Save["gAim"]["autoshoot"] = gCvars.aimbot_autoshoot;
        Save["gAim"]["zoomedonly"] = gCvars.aimbot_zoomedonly;
        Save["gAim"]["autoscope"] = gCvars.aimbot_autoscope;
        Save["gAim"]["fov"] = gCvars.aimbot_fov;
        /*
        * Misc
        */
        Save["gMisc"]["bhop"] = gCvars.misc_bunnyhop;
        Save["gMisc"]["autostrafe"] = gCvars.misc_autostrafe;
        Save["gMisc"]["noisemakerspam"] = gCvars.misc_noisemaker_spam;
        Save["gMisc"]["requeue"] = gCvars.misc_autoqueue;
        //Save["gMisc"]["record"] = gCvars.misc_record; are u retarded lol dont save it
        Save["gMisc"]["replay"] = gCvars.misc_replay;
        Save["gMisc"]["micspam"] = gCvars.misc_micspam;
        Save["gMisc"]["chatspam"] = gCvars.misc_chatspam;
        Save["gMisc"]["thirdperson"] = gCvars.misc_thirdperson;
        Save["gMisc"]["namestealer"] = gCvars.misc_namestealer;
        /*
        * AA
        */
        Save["gAA"]["enable"] = gCvars.antiaim_enable;
        Save["gAA"]["pitch"] = gCvars.antiaim_pitch;
        Save["gAA"]["yaw"] = gCvars.antiaim_yaw;
        Save["gAA"]["spinspeed"] = gCvars.antiaim_spinspeed;
        output << Save;
        output.close();
    }
}

void CConfig::Load()
{
    std::ifstream load("realityhook.json");

    if (load.is_open()) 
    {
        Json::Value Load;
        load >> Load;
        /* Aim */
        gCvars.aimbot_active = Load["gAim"]["active"].asFloat();
        gCvars.aimbot_autoscope = Load["gAim"]["autoscope"].asFloat();
        gCvars.aimbot_hitbox = Load["gAim"]["hitbox"].asFloat();
        gCvars.aimbot_fov = Load["gAim"]["fov"].asFloat();
        gCvars.aimbot_autoshoot = Load["gAim"]["autoshoot"].asFloat();
        gCvars.aimbot_hitscan = Load["gAim"]["hitscan"].asFloat();
        gCvars.aimbot_zoomedonly = Load["gAim"]["zoomedonly"].asFloat();
        gCvars.aimbot_silent = Load["gAim"]["silent"].asFloat();
        /* Misc */
        gCvars.misc_bunnyhop = Load["gMisc"]["bhop"].asFloat();
        gCvars.misc_autostrafe = Load["gMisc"]["autostrafe"].asFloat();
        gCvars.misc_noisemaker_spam = Load["gMisc"]["noisemakerspam"].asFloat();
        gCvars.misc_autoqueue = Load["gMisc"]["requeue"].asFloat();
        gCvars.misc_replay = Load["gMisc"]["replay"].asFloat();
        gCvars.misc_micspam = Load["gMisc"]["micspam"].asFloat();
        gCvars.misc_chatspam = Load["gMisc"]["chatspam"].asFloat();
        gCvars.misc_thirdperson = Load["gMisc"]["thirdperson"].asFloat();
        gCvars.misc_namestealer = Load["gMisc"]["namestealer"].asFloat();
        /* AA */
        gCvars.antiaim_enable = Load["gAA"]["enable"].asFloat();
        gCvars.antiaim_pitch = Load["gAA"]["pitch"].asFloat();
        gCvars.antiaim_yaw = Load["gAA"]["yaw"].asFloat();
    }
}