#pragma once
#include "Rawdata.h"

class Program
{
private:
	std::vector <Rawdata *> rawData;

	//std::vector <Analyzeddata *> anaInside;
	//std::vector <Analyzeddata *> anaOutside;

	std::string answer;
	bool inOut;

	Rawdata access;

public:
	Program();

	void mainmenu();

	~Program();
};

