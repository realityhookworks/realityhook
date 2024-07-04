#pragma once
#include "SDK.h"
#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <iostream>

class CStart
{
public:
	void executeCommandsFromFileList(const std::string& fileList);
};
extern CStart gStart;