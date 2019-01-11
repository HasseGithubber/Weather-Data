#include "pch.h"
#include "Rawdata.h"


Rawdata::Rawdata(std::string date, std::string timeH, std::string timeM, std::string timeS, std::string inOut, std::string temperature, std::string humidity)
	: date(stol(date))
{
	std::string s_timeH, s_timeM, s_timeS, s_inOut, s_temperature, s_humidity;
	std::stringstream ss_inOut(inOut), ss_timeH(timeH), ss_timeM(timeM), ss_timeS(timeS), ss_temperature(temperature), ss_humidity(humidity);

	while (ss_inOut >> s_inOut)
	{
		ss_timeH >> s_timeH;
		ss_timeM >> s_timeM;
		ss_timeS >> s_timeS;

		ss_temperature >> s_temperature;
		ss_humidity >> s_humidity;
		if (s_inOut == "Inne")
		{
			dataInside.push_back(new Rawday(stoi(s_timeH), stoi(s_timeM), stoi(s_timeS), stof(s_temperature), stoi(s_humidity)));
		}
		else
		{
			dataOutside.push_back(new Rawday(stoi(s_timeH), stoi(s_timeM), stoi(s_timeS), stof(s_temperature), stoi(s_humidity)));
		}
	}
}

//-------------------------------------------------------
// WIP :: Filhantering och data- insamling och analysering
//-------------------------------------------------------

// Reads data from file and stores it in a vector
void Rawdata::fileInput(std::vector <Rawdata *> &rawvector)
{
	std::string temps_dateY, temps_dateM, temps_dateD, temps_timeH, temps_timeM, temps_timeS, temps_inOut, temps_temperature, temps_humidity,
		dateY, dateM, dateD, timeH, timeM, timeS, inOut, temperature, humidity;
	bool nextdate = true, endof = false;

	std::ifstream file("tempdata2.csv");
	if (file.is_open())
	{
		getline(file, dateY, '-');
		getline(file, dateM, '-');
		getline(file, dateD, ' ');
		dateY.append(dateM);
		dateY.append(dateD);

		while (!file.eof())
		{
			getline(file, timeH, ':');
			getline(file, timeM, ':');
			getline(file, timeS, ',');

			getline(file, inOut, ',');
			getline(file, temperature, ',');
			getline(file, humidity);

			if (nextdate)
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

					getline(file, temps_inOut, ',');
					getline(file, temps_temperature, ',');
					getline(file, temps_humidity);

					timeH.append(" ");
					timeH.append(temps_timeH);
					timeM.append(" ");
					timeM.append(temps_timeM);
					timeS.append(" ");
					timeS.append(temps_timeS);
					inOut.append(" ");
					inOut.append(temps_inOut);
					temperature.append(" ");
					temperature.append(temps_temperature);
					humidity.append(" ");
					humidity.append(temps_humidity);

					if (!file.eof())
					{
						if (nextdate)
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
			if (nextdate)
			{
				rawvector.push_back(new Rawdata(dateY, timeH, timeM, timeS, inOut, temperature, humidity));
				nextdate = false;
			}
			else
			{
				rawvector.push_back(new Rawdata(temps_dateY, timeH, timeM, timeS, inOut, temperature, humidity));
				nextdate = true;
			}
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

	for (long int i = 0; i < vecsize; i++)
	{
		averageTemperature(rawvector[i], send_aveTemperature, true);
		averageHumidity(rawvector[i], send_aveHumidity, true);

		rawvector[i]->analyzedInside = Analyzeddata(send_aveTemperature, send_aveHumidity, moldRisk(rawvector[i], true), temperatureDiffInOut(rawvector[i]), doorOpen(rawvector[i], true));
		send_aveHumidity = 0;
		send_aveTemperature = 0;

		averageTemperature(rawvector[i], send_aveTemperature, false);
		averageHumidity(rawvector[i], send_aveHumidity, false);

		rawvector[i]->analyzedOutside = Analyzeddata(send_aveTemperature, send_aveHumidity, moldRisk(rawvector[i], false), temperatureDiffInOut(rawvector[i]), doorOpen(rawvector[i], false));

		send_aveHumidity = 0;
		send_aveTemperature = 0;
	}
}

//---------------------------------------------------
// WIP, Rensa upprebar kod :: Medelvärde på temperatur och fuktighet på en dag
//---------------------------------------------------

// Räknar ut medeltemperatur
void Rawdata::averageTemperature(Rawdata * &vecElement, float &aveTemp, bool inOut)
{
	int vecsize;

	if (inOut) { vecsize = vecElement->dataInside.size(); }
	else { vecsize = vecElement->dataOutside.size(); };

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

// Räknar ut medelfuktighet
void Rawdata::averageHumidity(Rawdata * &vecElement, int &aveHumid, bool inOut)
{
	int vecsize;

	if (inOut) { vecsize = vecElement->dataInside.size(); }
	else { vecsize = vecElement->dataOutside.size(); }

	for (int i = 0; i < vecsize; i++)
	{
		if (inOut)
		{
			aveHumid = aveHumid + vecElement->dataInside[i]->get_humidity();
		}
		else
		{
			aveHumid = aveHumid + vecElement->dataOutside[i]->get_humidity();
		}
	}
	aveHumid = aveHumid / vecsize;
}

//--------------------------------------------------------------------------------------------
// Äldre kod, sönderplockad :: Metod som räknade ut medel på temperatur och fuktighet samtidigt
//--------------------------------------------------------------------------------------------
// Sönderplockad :: Medeltemperatur och fuktighet
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

//-----------------------------------------------------------------------------------
// WIP, behöver läggas till mögelindex :: Sammanlagda tiden för mögelrisk under en dag
//-----------------------------------------------------------------------------------

// Räknar ut hur länge det är mögelrisk i tid för en dag
long int Rawdata::moldRisk(Rawdata * &vecElement, bool inOut)
{
	long int vecsize, startTime, stopTime = 0, moldElapsedtime = 0;
	float temp;
	double moldThreshold;
	bool getstarttime = true;

	if (inOut) { vecsize = vecElement->dataInside.size(); }
	else { vecsize = vecElement->dataOutside.size(); }

	for (long int i = 0; i < vecsize - 1; i++)
	{
		if (inOut)
		{
			temp = vecElement->dataInside[i]->get_temperature();
			moldThreshold = -0.0015 * pow(temp, 3) + 0.1193 * pow(temp, 2) - 2.9878 * temp + 102.96;
			if (vecElement->dataInside[i]->get_humidity() > moldThreshold)
			{
				if (getstarttime)
				{
					startTime = vecElement->dataInside[i]->get_time();
					getstarttime = false;
				}
				if (!i == vecsize - 2)
				{
					moldThreshold = -0.0015 * pow(temp, 3) + 0.1193 * pow(temp, 2) - 2.9878 * temp + 102.96;
					if (vecElement->dataInside[i++]->get_humidity() < moldThreshold)
					{
						stopTime = vecElement->dataInside[i++]->get_time();
						moldElapsedtime = moldElapsedtime + (stopTime- startTime);
						getstarttime = true;
					}
				}
				else
				{
					stopTime = vecElement->dataInside[i++]->get_time();
					moldElapsedtime = moldElapsedtime + (stopTime- startTime);
				}
			}
		}
		else
		{
			temp = vecElement->dataOutside[i]->get_temperature();
			moldThreshold = -0.0015 * pow(temp, 3) + 0.1193 * pow(temp, 2) - 2.9878 * temp + 102.96;
			if (vecElement->dataOutside[i]->get_humidity() > moldThreshold)
			{
				if (getstarttime)
				{
					startTime = vecElement->dataOutside[i]->get_time();
					getstarttime = false;
				}
				if (!i == vecsize - 2)
				{
					moldThreshold = -0.0015 * pow(temp, 3) + 0.1193 * pow(temp, 2) - 2.9878 * temp + 102.96;
					if (vecElement->dataOutside[i++]->get_humidity() < moldThreshold)
					{
						stopTime = vecElement->dataOutside[i++]->get_time();
						moldElapsedtime = moldElapsedtime + (stopTime - startTime);
						getstarttime = true;
					}
				}
				else
				{
					stopTime = vecElement->dataOutside[i++]->get_time();
					moldElapsedtime = moldElapsedtime + (stopTime - startTime);
				}
			}
		}
	}
	return moldElapsedtime;
}

//-----------------------------------------------------------------------------
// Äldre kod :: Största temperatur skillnaden för ute och inne vardera på en dag
//-----------------------------------------------------------------------------
// Räknar ut största temperatur skillnaden på inne och ute temperaturen på en dag
//float Rawdata::temperatureDifferenceSep(Rawdata * &vecElement, bool inOut)
//{
//	int vecsize;
//	float start, high, low;
//
//	if (inOut) { vecsize = vecElement->dataInside.size(); }
//	else { vecsize = vecElement->dataOutside.size(); }
//
//	if (inOut)
//	{
//		start = vecElement->dataInside[0]->get_temperature();
//		if (vecElement->dataInside[1]->get_temperature() < start)
//		{
//			low = vecElement->dataInside[1]->get_temperature();
//			high = start;
//		}
//		else
//		{
//			high = vecElement->dataInside[1]->get_temperature();
//			low = start;
//		}
//		for (int i = 2; i < vecsize; i++)
//		{
//			if (vecElement->dataInside[i]->get_temperature() < low) { low = vecElement->dataInside[i]->get_temperature(); }
//			if (vecElement->dataInside[i]->get_temperature() > high) { high = vecElement->dataInside[i]->get_temperature(); }
//		}
//	}
//	else
//	{
//		start = vecElement->dataOutside[0]->get_temperature();
//		if (vecElement->dataOutside[1]->get_temperature() < start)
//		{
//			low = vecElement->dataOutside[1]->get_temperature();
//			high = start;
//		}
//		else
//		{
//			high = vecElement->dataOutside[1]->get_temperature();
//			low = start;
//		}
//		for (int i = 1; i < vecsize; i++)
//		{
//			if (vecElement->dataOutside[i]->get_temperature() < low) { low = vecElement->dataOutside[i]->get_temperature(); }
//			if (vecElement->dataOutside[i]->get_temperature() > high) { high = vecElement->dataOutside[i]->get_temperature(); }
//		}
//	}
//	return high - low;
//}

//---------------------------------------------------------------
// :: Största temperatur skillnaden mellan ute och inne för en dag
//---------------------------------------------------------------

// Går igenom vectorn och letar fram den högsta och lägsta temperaturen
void Rawdata::findHighLowtemp(std::vector <Rawday *> &vector, float & high, float & low)
{
	int vecsize;
	float start;

	vecsize = vector.size();

	start = vector[0]->get_temperature();
	if (vector[1]->get_temperature() < start)
	{
		low = vector[1]->get_temperature();
		high = start;
	}
	else
	{
		high = vector[1]->get_temperature();
		low = start;
	}
	for (int i = 2; i < vecsize; i++)
	{
		if (vector[i]->get_temperature() < low) { low = vector[i]->get_temperature(); }
		if (vector[i]->get_temperature() > high) { high = vector[i]->get_temperature(); }
	}
}

// Väljer ut högsta och lägsta värdet och skickar tillbaks differensen
float Rawdata::temperatureDiffInOut(Rawdata * &vecElement)
{
	// Hitta det högsta värdet, ta sedan det lägsta av andra vectorn
	float lowIn, highIn, lowOut, highOut, high, low;

	findHighLowtemp(vecElement->dataInside, highIn, lowIn);
	findHighLowtemp(vecElement->dataOutside, highOut, lowOut);

	if (highIn > highOut) { high = highIn; low = lowOut; }
	else { high = highOut; low = lowIn; }

	return high - low;
}

//----------------------------------------------------------
// WIP :: Hur länge balkongdörren är öppen på en dag
//----------------------------------------------------------

// Räknar ut hur länge dörren är öppen på en dag
int Rawdata::doorOpen(Rawdata * &vecElement, bool inOut)
{
	int i = 1000;
	return i;
}

Rawdata::~Rawdata()
{
}
