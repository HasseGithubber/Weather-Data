#pragma once
class Rawday
{
private:

	int hour;
	int minutes;
	int seconds;
	//bool inOut;
	float temperature;
	int humidity;

public:

	Rawday();
	Rawday(int hour, int minutes, int seconds, float temperature, int humidity) 
		: hour(hour), minutes(minutes), seconds(seconds), temperature(temperature), humidity(humidity) {};

	int get_hour() { return this->hour; }
	int get_minutes() { return this->minutes; }
	int get_seconds() { return this->seconds; }
	float get_temperature() { return this->temperature; }
	int get_humidity() { return this->humidity; }

	~Rawday();
};
