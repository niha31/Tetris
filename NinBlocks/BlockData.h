#pragma once

#include "Constants.h"

#include <iostream>
#include <fstream>

class BlockData
{
private:
	int colourID;
	std::string name;

public:
	int*** data;

	BlockData(std::string name);
	~BlockData();

	bool Load();
	void UnLoad();
};

