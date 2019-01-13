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

	// Data hantering
	void fileInput(std::vector <Rawdata *> &);
	void convertData(std::vector <Rawdata *> &);

	// Data kalkylering
	void averageTemperature(Rawdata * &, float &, bool);
	void averageHumidity(Rawdata * &, int &, bool);
	
	void moldRisk(std::vector <Rawday *> &, int &, double &, double &);
	//long int oldmoldRisk(Rawdata * &, bool);

	//testest
	float temperatureDifferenceSep(Rawdata * &, bool);

	void findHighLowtemp(std::vector <Rawday *> &, float &, float &);
	float temperatureDiffInOut(Rawdata * &);
	int doorOpen(Rawdata * &, bool);

	~Rawdata();
};
