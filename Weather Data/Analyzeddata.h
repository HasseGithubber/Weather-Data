#pragma once
class Analyzeddata
{
private:

	int moldrisk_time,
		doorOpen_time;
	float aveTemperature,
		aveMoldIndex,
		aveMoldIndex_time,
		temperatureDiff;
	int aveHumidity;

public:

	Analyzeddata();
	Analyzeddata(float aveTemperature, int aveHumidity, float avemoldIndex, int moldrisk_time, float avemoldindex_time, float temperatureDiff, int doorOpen_time)
		: aveTemperature(aveTemperature), aveHumidity(aveHumidity), aveMoldIndex(avemoldIndex), moldrisk_time(moldrisk_time), aveMoldIndex_time(avemoldindex_time), temperatureDiff(temperatureDiff), doorOpen_time(doorOpen_time) {};

	//void set_aveTemperature(float temp) { aveTemperature = temp; }
	//void set_aveHumidity(int humid) { aveHumidity = humid; }

	float get_aveTemperature() { return this->aveTemperature; }
	float get_aveMoldIndex() { return this->aveMoldIndex; }
	float get_aveTemperatureDiff() { return this->temperatureDiff; }
	int get_aveHumidity() { return this->aveHumidity; }
	int get_doorOpen() { return this->doorOpen_time; }

	// Converting back to time
	// Seconds / 3600 % 60 (int) = hours
	// Seconds / 60 % 60 (int) = minutes
	// Seconds % 60 (int) = seconds

	~Analyzeddata();
};

