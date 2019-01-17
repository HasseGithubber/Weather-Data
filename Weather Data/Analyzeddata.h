#pragma once

class Analyzeddata
{
private:

	/*This is the class I make an instance of for each day to hold it's average values,
	mold index and other information.
	I saved the moldrisk_time in total seconds as taken from the raw data,
	the doorOpen is just saved in numbers of hours. I chose to save most values
	with decimals into floats but could definitely be switched into doubles
	for more precise outcomes. Humidity I went with a simple int as humidity is
	rarely presented with decimals, from what I could find*/
	int moldrisk_time,
		doorOpen_time;
	float aveTemperature,
		aveMoldIndex,
		aveMoldIndex_time,
		temperatureDiff;
	int aveHumidity;

public:

	/*Constructors*/
	Analyzeddata();
	Analyzeddata(float aveTemperature, int aveHumidity, float avemoldIndex, int moldrisk_time, float avemoldindex_time, float temperatureDiff, int doorOpen_time)
		: aveTemperature(aveTemperature), aveHumidity(aveHumidity), aveMoldIndex(avemoldIndex), moldrisk_time(moldrisk_time), aveMoldIndex_time(avemoldindex_time), temperatureDiff(temperatureDiff), doorOpen_time(doorOpen_time) {};

	/*Getters*/
	float get_aveTemperature() { return this->aveTemperature; }
	float get_aveMoldIndex() { return this->aveMoldIndex; }
	float get_aveTemperatureDiff() { return this->temperatureDiff; }
	float get_aveMoldIndTime() { return this->aveMoldIndex_time; }
	int get_moldriskTime() { return this->moldrisk_time; }
	int get_aveHumidity() { return this->aveHumidity; }
	int get_doorOpen() { return this->doorOpen_time; }

	// Converting back to time
	// Seconds / 3600 % 60 (int) = hours
	// Seconds / 60 % 60 (int) = minutes
	// Seconds % 60 (int) = seconds

	~Analyzeddata();
};

