#pragma once
class Analyzeddata
{
private:

	long int moldrisk_time, 
		doorOpen_time;
	float aveTemperature, 
		temperatureDiff;
	int aveHumidity;

public:

	Analyzeddata() {};
	Analyzeddata(float aveTemperature, int aveHumidity, long int moldrisk_time, float temperatureDiff, long int doorOpen_time)
		: aveTemperature(aveTemperature), aveHumidity(aveHumidity), moldrisk_time(moldrisk_time), temperatureDiff(temperatureDiff), doorOpen_time(doorOpen_time) {};

	~Analyzeddata();
};

