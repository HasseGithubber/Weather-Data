#pragma once
class Analyzeddata
{
private:

	long int moldrisk_time, 
		doorOpen_time;
	float aveTemperature, 
		temperatureDiffinside,
		temperatureDiffoutside;
	int aveHumidity;

public:

	Analyzeddata();
	Analyzeddata(float aveTemperature, int aveHumidity, long int moldrisk_time, float temperatureDiffin, float temperatureDiffout, long int doorOpen_time)
		: aveTemperature(aveTemperature), aveHumidity(aveHumidity), moldrisk_time(moldrisk_time), temperatureDiffinside(temperatureDiffin), temperatureDiffoutside(temperatureDiffout), doorOpen_time(doorOpen_time) {};

	void set_aveTemperature(float temp) { aveTemperature = temp; }
	void set_aveHumidity(int humid) { aveHumidity = humid; }

	~Analyzeddata();
};

