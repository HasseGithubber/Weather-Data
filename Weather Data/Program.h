#pragma once
#include "Rawdata.h"
#include "tempData.h"

class Program
{
private:

	/*Two temporary vectors, first is used for printing sorted data
	and the other for calculating when and how long a balcony door has been open*/
	std::vector <tempData> sortedVector;
	std::vector <tempData *> balconyDoorVector;

	/*The vector I use for storing all the data from the file and calculated answers*/
	std::vector <Rawdata *> rawData;

	/*Simple string used in menus for storing choices*/
	std::string answer;

	/*An object made to access Rawdata:: methods*/
	Rawdata access;

public:
	Program();

	/*The first method run to load in all the data from the file and afterwards
	start to do the convertion to useful information*/
	void loadProcessData();
	
	/*Menus*/
	void mainmenu();
	void insideOptions();
	void outsideOptions();
	void searchSeason();

	~Program();
};

