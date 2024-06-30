#pragma once
#include "SDK.h"

class CConfig
{
public:
	void Save();
	void Load();
};
extern CConfig gConfig;