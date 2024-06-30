#include "DUM.h"
#include <fstream>
#include <ctime>
static int anti_auto_balance = 0;
static const char* previous_server = { };

bool __fastcall dispatch_user_message(void* ecx, void* edx, int msg_type, bf_read& msg_data)
{
	VMTManager& hook = VMTManager::GetHook(gInts.Client);
	auto data = reinterpret_cast<const char*>(msg_data.m_pData);
	msg_data.SetAssertOnOverflow(false);

	if (data)
	{
		switch (msg_type)
		{
		case SayText2:
		{
			const int nbl = msg_data.GetNumBytesLeft();
			if (nbl < 5 || nbl >= 256)
			{
				break;
			}

			const int entIdx = msg_data.ReadByte();
			msg_data.Seek(8);
			char typeBuffer[256], nameBuffer[256], msgBuffer[256];
			if (msg_data.GetNumBytesLeft() == 0) { break; }
			msg_data.ReadString(typeBuffer, 256);
			if (msg_data.GetNumBytesLeft() == 0) { break; }
			msg_data.ReadString(nameBuffer, 256);
			if (msg_data.GetNumBytesLeft() == 0) { break; }
			msg_data.ReadString(msgBuffer, 256);

			std::string chatType(typeBuffer);
			std::string playerName(nameBuffer);
			std::string chatMessage(msgBuffer);
			std::ofstream file("chat_log.txt", std::ios::app);
			if (file.is_open()) {
				std::string logEntry = playerName + ":" + chatMessage + "\n";

				// write it
				file << logEntry;
				file.close();
			}
			else {
				// nothing :3
			}

			break;
		}
		case VGUIMenu:
		{
			
			if (strncmp(reinterpret_cast<char*>(msg_data.m_pData), "class_", 6) == 0)
			{
				gInts.Engine->ClientCmd_Unrestricted("join_class sniper");
				return true;
			}
			if (strcmp(reinterpret_cast<char*>(msg_data.m_pData), "info") == 0)
			{
				gInts.Engine->ClientCmd_Unrestricted("closedwelcomemenu");
				return true;
			}
			break;
		}
		case VoteStart:
		case VoteFailed:
		case VotePass:
		{
			//visuals->vote_revealer_register_message(msg_type, msg_data);
			break;
		}
		}
	}

	msg_data.Seek(0);
	return hook.GetMethod<bool(__fastcall*)(void*, void*, int, bf_read&)>(36)(ecx, edx, msg_type, msg_data);
}
