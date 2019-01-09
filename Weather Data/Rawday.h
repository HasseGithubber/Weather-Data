#pragma once
class Rawday
{
private:

	long int time;
	//bool inOut;
	float temperature;
	int humidity;

public:

	Rawday();
	Rawday(long int time, float temperature, int humidity) 
		: time(time), temperature(temperature), humidity(humidity) {};

	long int get_time() { return this->time; }
	float get_temperature() { return this->temperature; }
	int get_humidity() { return this->humidity; }

	~Rawday();
};
