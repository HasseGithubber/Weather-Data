#pragma once
#include "Rawday.h"
#include "Analyzeddata.h"
#include "tempData.h"

#include <fstream>
#include <sstream>
#include <algorithm>
#include <stdlib.h>

class Rawdata
{
private:

	/*I used enums to better keep track on the type of data the user wanted from
	the choice they made in the menu*/
	enum valueType
	{
		e_aveTemperature, e_aveHumidity, e_aveMoldIndex, e_temperatureDiff, e_doorOpen
	};

	/*I decided later on to save the date as a string for easier printing and it's still able to be
	searched through with a binary search I added later.
	The temperatureDiff variable is the difference between the highest difference between
	the inside temperature and outside temperature for one day*/
	std::string date;
	float temperatureDiff;

	/*As I didn't want to make checks in the program for similar days through a vector I decided
	to save each day as one element in a vector of this class.
	Each day then contains two vectors for all the data that belongs to that date
	divided into their own inside and outside vectors*/
	std::vector <Rawday *> dataInside;
	std::vector <Rawday *> dataOutside;

	/*These are the objects that collects all the calculated data,
	each day has it's own average temperature, humidity and so on stored in these*/
	Analyzeddata analyzedInside;
	Analyzeddata analyzedOutside;

public:

	/*Constructors*/
	Rawdata() {};
	Rawdata(std::string, std::string, std::string, std::string, std::string, std::string, std::string);

	/*Getters*/
	std::string get_date() { return this->date; }
	float get_temperatureDiff() { return this->temperatureDiff; }

	/*File and data conversion head methods*/
	void fileInput(std::vector <Rawdata *> &);
	void convertData(std::vector <Rawdata *> &, std::vector <tempData *> &);

	/*Average temperature and humidity calculations*/
	void averageTempHumid(Rawdata * &, float &, int &, bool);
	
	/*Average mold index and a method for the amount of time
	there is risk for mold duriong a day an it's own corresponding mold index*/
	void aveMold_Index(std::vector <Rawday *> &, double &);
	void moldRisk_time(std::vector <Rawday *> &, int &, double &);

	/*Old code saved for myself*/
	//float temperatureDifferenceSep(Rawdata * &, bool);

	/*Calculates the highest difference between in and out temperature*/
	void findHighLowtemp(std::vector <Rawday *> &, float &, float &);
	float temperatureDiffInOut(Rawdata * &);

	/*An algorithm to check how long the door has been open during a day in hours*/
	void hourlyAverageTemp(std::vector <tempData *> &, std::vector <Rawday *> &, std::vector <Rawday *> &);
	void doorOpen(std::vector <tempData *> &, Rawdata * &, int &);

	/*A binary search for searching the vector for a date*/
	int binarySearch(std::vector <Rawdata *> &, int, int, std::string);
	void searchDate(std::vector <Rawdata *> &, std::string, bool);

	/*Searches for a date when a season starts*/
	void SearchSeason(std::vector <Rawdata *> &, float);

	/*Comparers for the stable sort*/
	static bool compareFloat(const tempData &, const tempData &);
	static bool compareInt(const tempData &, const tempData &);

	/*The methods used for storing the wanted data temporarly in a vector,
	sort it and print the top 5 highest and lowest values*/
	void sortData(std::vector <tempData> &, std::vector <Rawdata *> &, int, bool);
	void sortData_choice(std::vector <tempData> &, Rawdata * &, Analyzeddata &, int &);
	void printVectorTop(std::vector <tempData> &, std::string, bool);

	/*Deletes vectors when shutting down*/
	void deleteVectors(std::vector <Rawdata *> &);

	/*Deconstructor*/
	~Rawdata();
};
