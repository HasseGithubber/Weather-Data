#pragma once
#include "Rawday.h"
#include "Analyzeddata.h"

#include <fstream>
#include <sstream>

class Rawdata
{
private:

	long int date;
	float temperatureDiff;

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
	long int moldRisk(Rawdata * &, bool);
	void findHighLowtemp(std::vector <Rawday *> &, float &, float &);
	float temperatureDiffInOut(Rawdata * &);
	int doorOpen(Rawdata * &, bool);

	~Rawdata();
};
