#include "BlockData.h"



BlockData::BlockData(std::string name)
{
	this->name = name;
	Load();
}


BlockData::~BlockData()
{
	UnLoad();
}

bool BlockData::Load() {
	std::ifstream dataFile("Blocks/" + name + "/data.txt");

	data = new int**[4];
	for (int k = 0; k < 4; k++) {
		data[k] = new int*[4];
		for (int j = 0; j < 4; j++) {
			data[k][j] = new int[4];
		}
	}

	if (dataFile.good()) {
		for (int k = 0; k < 4; k++) {
			for (int j = 0; j < 4; j++) {
				for (int i = 0; i < 4; i++) {
					dataFile >> data[k][j][i];
				}
			}
		}
		dataFile >> colourID;
		dataFile.close();
		return true;
	}
	std::cout << ("Failed to open BlockData file: " + name).c_str() << std::endl;
	return false;
}

void BlockData::UnLoad() { 
	for (int k = 0; k < 4; k++) {
		for (int j = 0; j < 4; j++) {
			delete[] data[k][j];
		}
		delete[] data[k];
	}
	delete[] data;
}