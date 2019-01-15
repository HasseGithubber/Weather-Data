#pragma once
class tempData
{
private:
	std::string date;
	int i_valuetoSort;
	float f_valuetoSort;
public:
	tempData();

	tempData(std::string date, float value) : date(date), f_valuetoSort(value) {};
	tempData(std::string date, int value) : date(date), i_valuetoSort(value) {};

	std::string get_date() { return this->date; };
	int get_intValue() const { return this->i_valuetoSort; };
	float get_floatValue() const { return this->f_valuetoSort; };

	~tempData();
};
