#pragma once
#include "Time.h"

class Rawday
{
private:

	/*This is where I make an instance for storing one slice of data from the file,
	time is converted into and stored into total seconds which i found alot easier that a single int*/
	int time;
	float temperature;
	int humidity;

public:

	/*Constructors*/
	Rawday();
	Rawday(int hour, int minutes, int seconds, float temperature, int humidity) 
		: temperature(temperature), humidity(humidity)
	{
		time = hour * 3600 + minutes * 60 + seconds;
	};

	// Converting back to time
	// Seconds / 3600 % 60 (int) = hours
	// Seconds / 60 % 60 (int) = minutes
	// Seconds % 60 (int) = seconds

	/*Getters*/
	int get_time() { return this->time; }
	float get_temperature() { return this->temperature; }
	int get_humidity() { return this->humidity; }

	~Rawday();
};
