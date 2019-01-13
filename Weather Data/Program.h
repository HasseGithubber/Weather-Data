#pragma once
#include "Rawdata.h"

class Program
{
private:

	std::vector <Rawdata *> rawData;

	std::string answer;
	//bool inOut;

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

