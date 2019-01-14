#pragma once
#include "Rawdata.h"
#include "tempData.h"

class Program
{
private:

	std::vector <tempData> sortedVector;
	std::vector <Rawdata *> rawData;

	std::string answer;

	Rawdata access;

public:
	Program();

	void loadProcessData();
	void mainmenu();
	void insideOptions();
	void outsideOptions();
	void searchSeason();

	~Program();
};

