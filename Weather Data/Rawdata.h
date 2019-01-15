#pragma once
#include "Rawday.h"
#include "Analyzeddata.h"
#include "tempData.h"

#include <fstream>
#include <sstream>
#include <algorithm>

class Rawdata
{
private:

	enum valueType
	{
		e_aveTemperature, e_aveHumidity, e_aveMoldIndex, e_temperatureDiff, e_doorOpen
	};

	std::string date;
	int i_date;
	float temperatureDiff;

	std::vector <Rawday *> dataInside;
	std::vector <Rawday *> dataOutside;

	Analyzeddata analyzedInside;
	Analyzeddata analyzedOutside;

public:

	//Konstruktors
	Rawdata() {};
	Rawdata(std::string, std::string, std::string, std::string, std::string, std::string, std::string);

	// Getter
	std::string get_date() { return this->date; }
	int get_i_date() { return this->i_date; }
	float get_temperatureDiff() { return this->temperatureDiff; }

	// Data hantering
	void fileInput(std::vector <Rawdata *> &);
	void convertData(std::vector <Rawdata *> &);

	// Data kalkylering
	void averageTemperature(Rawdata * &, float &, bool);
	void averageHumidity(Rawdata * &, int &, bool);
	
	void aveMold_Index(std::vector <Rawday *> &, double &);
	void moldRisk_time(std::vector <Rawday *> &, int &, double &);

	//testest
	float temperatureDifferenceSep(Rawdata * &, bool);

	void findHighLowtemp(std::vector <Rawday *> &, float &, float &);
	float temperatureDiffInOut(Rawdata * &);
	int doorOpen(std::vector <Rawdata *> &, bool);

	// Bin�r s�kning efter datum
	int binarySearch(std::vector <Rawdata *> &, int, int, std::string);
	void searchDate(std::vector <Rawdata *> &, std::string, bool);

	// S�kning
	void SearchSeason(std::vector <Rawdata *> &, float);

	// Comparers
	static bool compareFloat(const tempData &, const tempData &);
	static bool compareInt(const tempData &, const tempData &);

	// Sortering
	void sortData(std::vector <tempData> &, std::vector <Rawdata *> &, int, bool);
	void sortData_choice(std::vector <tempData> &, Rawdata * &, Analyzeddata &, int &);
	void printVectorTop(std::vector <tempData> &, std::string, bool);

	// Dekonstruktor
	~Rawdata();
};
