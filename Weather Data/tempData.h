#pragma once
class tempData
{
private:
	int date,
		i_valuetoSort;
	float f_valuetoSort;
public:
	tempData();

	tempData(int date, float value) : date(date), f_valuetoSort(value) {};
	tempData(int date, int value) : date(date), i_valuetoSort(value) {};

	int get_date() { return this->date; };
	int get_intValue() { return this->i_valuetoSort; };
	float get_floatValue() { return this->f_valuetoSort; };

	~tempData();
};

