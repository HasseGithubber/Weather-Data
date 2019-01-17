#pragma once
class tempData
{
private:
	
	// Variables for sorting data
	std::string date;
	int i_valuetoSort;
	float f_valuetoSort;

	// Variables for the balcony door function
	//int hour_inseconds;
	float tempInside, tempOutside, tempDifference,
		humidInside, humidOutside, humidDifference;

public:
	tempData();

	// Constructor for balcony door vector
	tempData(float tempInside, float tempOutside, float tempDifference, float humidInside, float humidOutside, float humidDifference) 
		: tempInside(tempInside), tempOutside(tempOutside), tempDifference(tempDifference), humidInside(humidInside), humidOutside(humidOutside), humidDifference(humidDifference) {};
	
	// Constructors for sorting data
	tempData(std::string date, float value) : date(date), f_valuetoSort(value) {};
	tempData(std::string date, int value) : date(date), i_valuetoSort(value) {};

	// Getters
	std::string get_date() { return this->date; };
	int get_intValue() const { return this->i_valuetoSort; };
	float get_floatValue() const { return this->f_valuetoSort; };

	float get_tempInside() { return this->tempInside; }
	float get_tempOutside() { return this->tempOutside; }
	float get_humidInside() { return this->humidInside; }
	float get_humidOutside() { return this->humidOutside; }
	float get_tempDifference() { return this->tempDifference; }
	float get_humidDifference() { return this->humidDifference; }

	~tempData();
};
