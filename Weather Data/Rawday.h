#pragma once
#include "Time.h"

class Rawday
{
private:

	int time;
	float temperature;
	int humidity;

public:

	Rawday();
	Rawday(int hour, int minutes, int seconds, float temperature, int humidity) 
		: temperature(temperature), humidity(humidity)
	{
		time = hour * 3600 + minutes * 60 + seconds;
	};

	int get_time() { return this->time; }
	float get_temperature() { return this->temperature; }
	int get_humidity() { return this->humidity; }

	~Rawday();
};
