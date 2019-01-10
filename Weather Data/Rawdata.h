#pragma once
#include "Rawday.h"
#include "Analyzeddata.h"

#include <fstream>
#include <sstream>

class Rawdata
{
private:

	long int date;
	std::vector <Rawday *> dataInside;
	std::vector <Rawday *> dataOutside;

	Analyzeddata analyzedInside;
	Analyzeddata analyzedOutside;

public:

	//Konstruktors
	Rawdata() {};
	Rawdata(std::string, std::string, std::string, std::string, std::string, std::string, std::string);

	long int get_date() { return this->date; }

	void fileInput(std::vector <Rawdata *> &);

	void convertData(std::vector <Rawdata *> &);
	void averageTemperature(Rawdata * &, float &, bool);
	void averageHumidity(Rawdata * &, int &, bool);
	//void averageValues(std::vector <Rawdata *> &, long int &, float &, int &, bool);
	long int moldRisk(Rawdata * &, bool);
	float temperatureDifference(Rawdata * &, bool);
	int doorOpen(Rawdata * &, bool);

	~Rawdata();
};
