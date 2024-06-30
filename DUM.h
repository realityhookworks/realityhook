#pragma once
#include "SDK.h"
#include "bitbuff.h"

bool __fastcall dispatch_user_message(void* ecx, void* edx, int msg_type, bf_read& msg_data);