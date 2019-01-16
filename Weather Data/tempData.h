#pragma once
class tempData
{
private:
	
	// Variables for sorting data
	std::string date;
	int i_valuetoSort;
	float f_valuetoSort;

	// Variables for the balcony door function
	int hour_inseconds;
	float f_inside, f_outside, f_difference;

public:
	tempData();

	// Constructor for balcony door vector
	tempData(int hour, float inside, float outside, float difference) 
		: hour_inseconds(hour), f_inside(inside), f_outside(outside), f_difference(difference) {};
	
	// Constructors for sorting data
	tempData(std::string date, float value) : date(date), f_valuetoSort(value) {};
	tempData(std::string date, int value) : date(date), i_valuetoSort(value) {};

	// Getters
	std::string get_date() { return this->date; };
	int get_intValue() const { return this->i_valuetoSort; };
	float get_floatValue() const { return this->f_valuetoSort; };

	~tempData();
};
