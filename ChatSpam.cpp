#include "ChatSpam.h"
#include <chrono>
#include <random>
#include <sstream>
#include <string>
#include <fstream>
#include <map>
#include <cstdlib>
#include <iostream>

CSpam gSpam;

void EnsureSpamFileExists() {
    std::ifstream infile("spam.txt");
    if (!infile.good()) {
        std::ofstream outfile("spam.txt");
        outfile << "dsc.gg/realityhook | WE ARE NOT GONE" << std::endl;
        outfile << "dsc.gg/realityhook | WE WILL ARE BACK" << std::endl;
        outfile << "dsc.gg/realityhook | TRANNIES OF TF2 OWNED LOL" << std::endl;
        outfile << "dsc.gg/realityhook | WINDOWS BOT-HOSTING" << std::endl;
        outfile << "dsc.gg/realityhook | IM GOING TO BOMB VALVE HQ LOL" << std::endl;
        outfile << "dsc.gg/realityhook | YOU CANT GET RID OF US LOL" << std::endl;
        outfile.close();
    }
    infile.close();
}

std::string GetSpam() {
    EnsureSpamFileExists();

    std::ifstream spamFile("spam.txt");
    std::string str;

    if (spamFile.is_open()) {
        std::getline(spamFile, str);
        spamFile.close();
    }
    else {
        str = "h"; // Fallback in case the file can't be read
    }

    return "say " + str;
}

void CSpam::Run() 
{
    if (gCvars.misc_chatspam) {
        float flCurTime = gInts.Engine->Time();
        static float flNextSend = 0.0f;

        if (flCurTime > flNextSend) {
            gInts.Engine->ClientCmd_Unrestricted(GetSpam().c_str());
            flNextSend = (flCurTime + 8.0f); 
        }
    }
}
