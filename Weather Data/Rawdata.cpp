#include "pch.h"
#include "Rawdata.h"


Rawdata::Rawdata(std::string date, std::string time, std::string inOut, std::string temperature, std::string humidity) : date(stol(date))
{
	std::string s_time, s_inOut, s_temperature, s_humidity;
	std::stringstream ss_inOut(inOut), ss_time(time), ss_temperature(temperature), ss_humidity(humidity);
	//long int li_time;

	while (ss_inOut >> s_inOut)
	{
		ss_time >> s_time;
		//li_time = stol(s_time) * 360;
		ss_temperature >> s_temperature;
		ss_humidity >> s_humidity;
		if (s_inOut == "Inne")
		{
			dataInside.push_back(new Rawday(stol(s_time), stof(s_temperature), stoi(s_humidity)));
		}
		else
		{
			dataOutside.push_back(new Rawday(stol(s_time), stof(s_temperature), stoi(s_humidity)));
		}
	}
}

// Reads data from file and stores it in a vector
void Rawdata::fileInput(std::vector <Rawdata *> &rawvector)
{
	std::string temps_dateY, temps_dateM, temps_dateD, temps_timeH, temps_timeM, temps_timeS, temps_inOut, temps_temperature, temps_humidity,
		dateY, dateM, dateD, timeH, timeM, timeS, inOut, temperature, humidity;
	bool jump = true, nextdate = false, endof = false;

	std::ifstream file("tempdata2mindre.txt");
	if (file.is_open())
	{
		getline(file, dateY, '-');
		getline(file, dateM, '-');
		getline(file, dateD, ' ');
		dateY.append(dateM);
		dateY.append(dateD);

		while (!file.eof())
		{
			while (!nextdate)
			{
				getline(file, timeH, ':');
				getline(file, timeM, ':');
				getline(file, timeS, ',');
				timeH.append(timeM);
				timeH.append(timeS);

				getline(file, inOut, ',');
				getline(file, temperature, ',');
				getline(file, humidity);

				if (jump)
				{
					getline(file, temps_dateY, '-');
					getline(file, temps_dateM, '-');
					getline(file, temps_dateD, ' ');
					temps_dateY.append(temps_dateM);
					temps_dateY.append(temps_dateD);
				}
				else
				{
					getline(file, dateY, '-');
					getline(file, dateM, '-');
					getline(file, dateD, ' ');
					dateY.append(dateM);
					dateY.append(dateD);
				}
				while (dateY == temps_dateY && !endof)
				{
					if (!file.eof())
					{
						getline(file, temps_timeH, ':');
						getline(file, temps_timeM, ':');
						getline(file, temps_timeS, ',');
						temps_timeH.append(temps_timeM);
						temps_timeH.append(temps_timeS);

						getline(file, temps_inOut, ',');
						getline(file, temps_temperature, ',');
						getline(file, temps_humidity);

						timeH.append(" ");
						timeH.append(temps_timeH);
						inOut.append(" ");
						inOut.append(temps_inOut);
						temperature.append(" ");
						temperature.append(temps_temperature);
						humidity.append(" ");
						humidity.append(temps_humidity);

						if (!file.eof())
						{
							if (jump)
							{
								getline(file, temps_dateY, '-');
								getline(file, temps_dateM, '-');
								getline(file, temps_dateD, ' ');
								temps_dateY.append(temps_dateM);
								temps_dateY.append(temps_dateD);
							}
							else
							{
								getline(file, dateY, '-');
								getline(file, dateM, '-');
								getline(file, dateD, ' ');
								dateY.append(dateM);
								dateY.append(dateD);
							}
						}
					}
					else
					{
						endof = true;
					}
				}
				nextdate = true;
			}
			if (jump)
			{
				rawvector.push_back(new Rawdata(dateY, timeH, inOut, temperature, humidity));
				jump = false;
			}
			else
			{
				rawvector.push_back(new Rawdata(temps_dateY, timeH, inOut, temperature, humidity));
				jump = true;
			}
			nextdate = false;
		}
	}
	file.close();
}

// Converts the raw data into analyzed data
void Rawdata::convertData(std::vector <Rawdata *> &rawvector)
{
	float send_aveTemperature = 0;
	int send_aveHumidity = 0;

	long int vecsize = rawvector.size();

	// skicka rawvector[i] direkt istället för referens på hela och välja i efterhand?
	for (long int i = 0; i < vecsize; i++)
	{
		averageTemperature(rawvector[i], send_aveTemperature, true);
		rawvector[i]->analyzedInside.set_aveTemperature(send_aveTemperature);
		averageHumidity(rawvector[i], send_aveHumidity, true);
		rawvector[i]->analyzedInside.set_aveHumidity(send_aveHumidity);
		//rawvector[i]->analyzedInside(send_aveTemperature, send_aveHumidity, moldRisk(rawvector[i], true), temperatureDifference(rawvector[i], true), doorOpen(rawvector[i], true));

		send_aveHumidity = 0;
		send_aveTemperature = 0;

		averageTemperature(rawvector[i], send_aveTemperature, false);
		rawvector[i]->analyzedOutside.set_aveTemperature(send_aveTemperature);
		averageHumidity(rawvector[i], send_aveHumidity, false);
		rawvector[i]->analyzedOutside.set_aveHumidity(send_aveHumidity);
		//rawvector[i]->analyzedOutside(send_aveTemperature, send_aveHumidity, moldRisk(rawvector[i], false), temperatureDifference(rawvector[i], false), doorOpen(rawvector[i], false));

		send_aveHumidity = 0;
		send_aveTemperature = 0;
	}
}

//Räknar ut medeltemperatur
void Rawdata::averageTemperature(Rawdata * &vecElement, float &aveTemp, bool inOut)
{
	int vecsize;

	if (inOut) { vecsize = vecElement->dataInside.size(); }
	else { vecsize = vecElement->dataOutside.size(); };

	//LOG("Averagetemp size");
	//LOG(vecsize);

	for (int i = 0; i < vecsize; i++)
	{
		if (inOut)
		{
			aveTemp = aveTemp + vecElement->dataInside[i]->get_temperature();
		}
		else
		{
			aveTemp = aveTemp + vecElement->dataOutside[i]->get_temperature();
		}
	}
	aveTemp = aveTemp / vecsize;
}

// Räknar ut medel fuktighet
void Rawdata::averageHumidity(Rawdata * &vecElement, int &aveHumid, bool inOut)
{
	int vecsize;

	if (inOut) { vecsize = vecElement->dataInside.size(); }
	else { vecsize = vecElement->dataOutside.size(); }

	LOG("Averagehumid size");
	LOG(vecsize);

	for (int i = 0; i < vecsize; i++)
	{
		if (inOut)
		{
			LOG("Moist inne");
			LOG(aveHumid);
			aveHumid = aveHumid + vecElement->dataInside[i]->get_humidity();
		}
		else
		{
			LOG("Moist ute");
			LOG(aveHumid);
			aveHumid = aveHumid + vecElement->dataOutside[i]->get_humidity();
		}
	}
	aveHumid = aveHumid / vecsize;
}

// Sönder hackad metod som räknade ut medel på temperatur och fuktighet samtidigt

//void Rawdata::averageValues(std::vector <Rawdata *> &rawvector, long int &index, float &aveTemp, int &aveHumid, bool inOut)
//{
//	//long int vecsize;
//	//if (inOut) { vecsize = rawvector[index]->dataInside.size(); }
//	//else { vecsize = rawvector[index]->dataOutside.size(); }
//
//	for (long int i = 0; i < vecsize; i++)
//	{
//		if (inOut)
//		{
//			
//			aveHumid = aveHumid + rawvector[index]->dataInside[i]->get_humidity();
//		}
//		else
//		{
//			aveTemp = aveTemp + rawvector[index]->dataOutside[i]->get_temperature();
//			aveHumid = aveHumid + rawvector[index]->dataOutside[i]->get_humidity();
//		}
//	}
//}

// Räknar ut mögelrisk i tid för en dag --- Gör ny till en siffra.
long int Rawdata::moldRisk(Rawdata * &vecElement, bool inOut)
{
	long int vecsize, startTime, stopTime, moldElapsedtime;
	float temp;
	double moldThreshold;
	bool b_moldRisk;

	if (inOut) { vecsize = vecElement->dataInside.size(); }
	else { vecsize = vecElement->dataOutside.size(); }

	for (long int i = 0; i < vecsize; i++)
	{
		if (inOut)
		{
			temp = vecElement->dataInside[i]->get_temperature();
			moldThreshold = -0.0015 * pow(temp, 3) + 0.1193 * pow(temp, 2) - 2.9878 * temp + 102.96;
			if (vecElement->dataInside[i]->get_humidity() > moldThreshold)
			{
				if (!b_moldRisk) { startTime = vecElement->dataInside[i]->get_time(); }
				b_moldRisk = true;
			}
			else
			{
				if (b_moldRisk) { stopTime = vecElement->dataInside[i]->get_time(); }
				b_moldRisk = false;
				moldElapsedtime = moldElapsedtime + (stopTime * 360 - startTime * 360);
			}
		}
		else
		{
			temp = vecElement->dataOutside[i]->get_temperature();
			moldThreshold = -0.0015 * pow(temp, 3) + 0.1193 * pow(temp, 2) - 2.9878 * temp + 102.96;
			if (vecElement->dataOutside[i]->get_humidity() > moldThreshold)
			{
				if (!b_moldRisk) { startTime = vecElement->dataOutside[i]->get_time(); }
				b_moldRisk = true;
			}
			else
			{
				if (b_moldRisk) { stopTime = vecElement->dataOutside[i]->get_time(); }
				b_moldRisk = false;
				moldElapsedtime = moldElapsedtime = (stopTime * 360 - startTime * 360);
			}
		}
	}
	return moldElapsedtime / 360;
}

// Räknar ut temperatur skillnaden på inne och ute temperaturen per dag
float Rawdata::temperatureDifference(Rawdata * &vecElement, bool inOut)
{
	int vecsize;
	float start, high, low;

	if (inOut) { vecsize = vecElement->dataInside.size(); }
	else { vecsize = vecElement->dataOutside.size(); }

	if (inOut)
	{
		start = vecElement->dataInside[0]->get_temperature();
		if (vecElement->dataInside[1]->get_temperature() < start)
		{
			low = vecElement->dataInside[1]->get_temperature();
			high = start;
		}
		else
		{
			high = vecElement->dataInside[1]->get_temperature();
			low = start;
		}
		for (int i = 2; i < vecsize; i++)
		{
			if (vecElement->dataInside[i]->get_temperature() < low) { low = vecElement->dataInside[i]->get_temperature(); }
			if (vecElement->dataInside[i]->get_temperature() > high) { high = vecElement->dataInside[i]->get_temperature(); }
		}
	}
	else
	{
		start = vecElement->dataOutside[0]->get_temperature();
		if (vecElement->dataOutside[1]->get_temperature() < start)
		{
			low = vecElement->dataOutside[1]->get_temperature();
			high = start;
		}
		else
		{
			high = vecElement->dataOutside[1]->get_temperature();
			low = start;
		}
		for (int i = 1; i < vecsize; i++)
		{
			if (vecElement->dataOutside[i]->get_temperature() < low) { low = vecElement->dataOutside[i]->get_temperature(); }
			if (vecElement->dataOutside[i]->get_temperature() > high) { high = vecElement->dataOutside[i]->get_temperature(); }
		}
	}
	return high - low;
}

// Räknar ut hur länge dörren är öppen på en dag
int Rawdata::doorOpen(Rawdata * &vecElement, bool inOut)
{
	int i = 1000;
	return i;
}

Rawdata::~Rawdata()
{
}
