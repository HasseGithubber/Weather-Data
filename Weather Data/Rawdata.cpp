#include "pch.h"
#include "Rawdata.h"


Rawdata::Rawdata(std::string date, std::string timeH, std::string timeM, std::string timeS, std::string inOut, std::string temperature, std::string humidity)
	: date(date)
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
// WIP, st�das kanske :: Filhantering och data- insamling och analysering
//-------------------------------------------------------

// Reads data from file and stores it in a vector
void Rawdata::fileInput(std::vector <Rawdata *> &rawvector)
{
	std::string date, tempDate, temps_dateY, temps_dateM, temps_dateD, temps_timeH, temps_timeM, temps_timeS, temps_inOut, temps_temperature, temps_humidity,
		dateY, dateM, dateD, timeH, timeM, timeS, inOut, temperature, humidity;
	bool nextdate = true, endof = false;

	std::ifstream file("tempdata4.csv");
	if (file.is_open())
	{
		//getline(file, dateY, '-');
		//getline(file, dateM, '-');
		//getline(file, dateD, ' ');
		//dateY.append(dateM);
		//dateY.append(dateD);
		getline(file, date, ' ');

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
				//getline(file, temps_dateY, '-');
				//getline(file, temps_dateM, '-');
				//getline(file, temps_dateD, ' ');
				//temps_dateY.append(temps_dateM);
				//temps_dateY.append(temps_dateD);
				getline(file, tempDate, ' ');
			}
			else
			{
				//getline(file, dateY, '-');
				//getline(file, dateM, '-');
				//getline(file, dateD, ' ');
				//dateY.append(dateM);
				//dateY.append(dateD);
				getline(file, date, ' ');
			}
			while (date == tempDate && !endof)
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
							//getline(file, temps_dateY, '-');
							//getline(file, temps_dateM, '-');
							//getline(file, temps_dateD, ' ');
							//temps_dateY.append(temps_dateM);
							//temps_dateY.append(temps_dateD);
							getline(file, tempDate, ' ');
						}
						else
						{
							//getline(file, dateY, '-');
							//getline(file, dateM, '-');
							//getline(file, dateD, ' ');
							//dateY.append(dateM);
							//dateY.append(dateD);
							getline(file, date, ' ');
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
				rawvector.push_back(new Rawdata(date, timeH, timeM, timeS, inOut, temperature, humidity));
				nextdate = false;
			}
			else
			{
				rawvector.push_back(new Rawdata(tempDate, timeH, timeM, timeS, inOut, temperature, humidity));
				nextdate = true;
			}
		}
	}
	file.close();
}

// Converts the raw data into analyzed data
void Rawdata::convertData(std::vector <Rawdata *> &rawvector)
{
	float aveTemperature;
	int aveHumidity, moldRiskTime;
	double aveMoldIndexTime, aveMoldIndex;

	unsigned int vecsize = rawvector.size();

	for (unsigned int i = 0; i < vecsize; i++)
	{
		// testest - k�r bara inne separat --- M�ste �ndras till att s�tta nedre funktionen
		rawvector[i]->temperatureDiff = temperatureDifferenceSep(rawvector[i], true);

		averageTemperature(rawvector[i], aveTemperature, true);
		averageHumidity(rawvector[i], aveHumidity, true);

		//LOG("*_*_*_*_*_*INNE*_*_*_*_*_*_*");
		aveMold_Index(rawvector[i]->dataInside, aveMoldIndex);
		moldRisk_time(rawvector[i]->dataInside, moldRiskTime, aveMoldIndexTime);

		rawvector[i]->analyzedInside = Analyzeddata(aveTemperature, aveHumidity, aveMoldIndex, moldRiskTime, aveMoldIndexTime, temperatureDiffInOut(rawvector[i]), doorOpen(rawvector, true));

		averageTemperature(rawvector[i], aveTemperature, false);
		averageHumidity(rawvector[i], aveHumidity, false);

		//LOG("------------------UITE------------------");
		//LOG(rawvector[i]->get_date());
		aveMold_Index(rawvector[i]->dataOutside, aveMoldIndex);
		moldRisk_time(rawvector[i]->dataOutside, moldRiskTime, aveMoldIndexTime);

		rawvector[i]->analyzedOutside = Analyzeddata(aveTemperature, aveHumidity, aveMoldIndex, moldRiskTime, aveMoldIndexTime, temperatureDiffInOut(rawvector[i]), doorOpen(rawvector, false));
	}
}

//---------------------------------------------------
// :: Medelv�rde p� temperatur och fuktighet p� en dag
//---------------------------------------------------

// R�knar ut medeltemperatur
void Rawdata::averageTemperature(Rawdata * &vecElement, float &aveTemp, bool inOut)
{
	unsigned int vecsize;

	if (inOut) { vecsize = vecElement->dataInside.size(); }
	else { vecsize = vecElement->dataOutside.size(); };

	aveTemp = 0;
	for (unsigned int i = 0; i < vecsize; i++)
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

// R�knar ut medelfuktighet
void Rawdata::averageHumidity(Rawdata * &vecElement, int &aveHumid, bool inOut)
{
	unsigned int vecsize;

	if (inOut) { vecsize = vecElement->dataInside.size(); }
	else { vecsize = vecElement->dataOutside.size(); }

	aveHumid = 0;
	for (unsigned int i = 0; i < vecsize; i++)
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

//----------------------------------------------------------------------------------------------------------
// :: Sammanlagda tiden f�r m�gelrisk och dess medel-m�gelindex under en dag  och medel m�gelindex f�r hela dagen
//----------------------------------------------------------------------------------------------------------

// R�knar ut medel m�gelindex f�r hela dagen
void Rawdata::aveMold_Index(std::vector <Rawday *> &vector, double &aveMoldIndex)
{
	unsigned int vecsize = vector.size();
	float temperature;
	double humidMoldThreshold;

	aveMoldIndex = 0;

	for (unsigned int i = 0; i < vecsize; i++)
	{
		temperature = vector[i]->get_temperature();
		humidMoldThreshold = -0.0015 * pow(temperature, 3) + 0.1193 * pow(temperature, 2) - 2.9878 * temperature + 102.96;
		aveMoldIndex = aveMoldIndex + (vector[i]->get_humidity() - humidMoldThreshold);
	}
	aveMoldIndex = aveMoldIndex / vecsize;
}

// R�knar ut hur l�nge det �r risk f�r m�gel under en dag och dess medel m�gel index
void Rawdata::moldRisk_time(std::vector <Rawday *> &vector, int &moldRiskTime, double &aveMoldIndexTime)
{
	int startTime, stopTime, n = 0;
	unsigned int vecsize = vector.size();
	float temperature;
	double humidMoldThreshold;
	bool getstarttime = true;

	moldRiskTime = 0;
	aveMoldIndexTime = 0;

	for (unsigned int i = 0; i < vecsize - 1; i++)
	{
		temperature = vector[i]->get_temperature();
		humidMoldThreshold = -0.0015 * pow(temperature, 3) + 0.1193 * pow(temperature, 2) - 2.9878 * temperature + 102.96;
		if (vector[i]->get_humidity() > humidMoldThreshold)
		{
			aveMoldIndexTime = aveMoldIndexTime + (vector[i]->get_humidity() - humidMoldThreshold);
			//LOG("avemoldindex per tid");
			//LOG(aveMoldIndexTime);
			n++;
			//LOG(n);
			if (getstarttime)
			{
				startTime = vector[i]->get_time();
				getstarttime = false;
				//LOG("start tid");
				//LOG(startTime);
			}
			if (i < vecsize - 2)
			{
				temperature = vector[i + 1]->get_temperature();
				humidMoldThreshold = -0.0015 * pow(temperature, 3) + 0.1193 * pow(temperature, 2) - 2.9878 * temperature + 102.96;
				if (vector[i + 1]->get_humidity() < humidMoldThreshold)
				{
					stopTime = vector[i + 1]->get_time();
					//LOG("onmgoing stop tid");
					//LOG(stopTime);
					moldRiskTime = moldRiskTime + (stopTime - startTime);
					getstarttime = true;
				}
			}
			else
			{
				stopTime = vector[i + 1]->get_time();
				//LOG("slutliga stop tid");
				//LOG(stopTime);
				moldRiskTime = moldRiskTime + (stopTime - startTime);
			}
		}
	}
	if (n != 0)
	{
		aveMoldIndexTime = aveMoldIndexTime / n;
	}
}

//-----------------------------------------------------------------------------
// �ldre kod :: St�rsta temperatur skillnaden f�r ute och inne vardera p� en dag
//-----------------------------------------------------------------------------

// R�knar ut st�rsta temperatur skillnaden p� inne och ute temperaturen p� en dag

float Rawdata::temperatureDifferenceSep(Rawdata * &vecElement, bool inOut)
{
	unsigned int vecsize;
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
		for (unsigned int i = 2; i < vecsize; i++)
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
		for (unsigned int i = 1; i < vecsize; i++)
		{
			if (vecElement->dataOutside[i]->get_temperature() < low) { low = vecElement->dataOutside[i]->get_temperature(); }
			if (vecElement->dataOutside[i]->get_temperature() > high) { high = vecElement->dataOutside[i]->get_temperature(); }
		}
	}
	return high - low;
}

//---------------------------------------------------------------
// :: St�rsta temperatur skillnaden mellan ute och inne f�r en dag
//---------------------------------------------------------------

// G�r igenom vectorn och letar fram den h�gsta och l�gsta temperaturen
void Rawdata::findHighLowtemp(std::vector <Rawday *> &vector, float & high, float & low)
{
	unsigned int vecsize = vector.size();
	float start;

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
	for (unsigned int i = 2; i < vecsize; i++)
	{
		if (vector[i]->get_temperature() < low) { low = vector[i]->get_temperature(); }
		if (vector[i]->get_temperature() > high) { high = vector[i]->get_temperature(); }
	}
}

// V�ljer ut h�gsta och l�gsta v�rdet och skickar tillbaks differensen
float Rawdata::temperatureDiffInOut(Rawdata * &vecElement)
{
	float lowIn, highIn, lowOut, highOut, high, low;

	findHighLowtemp(vecElement->dataInside, highIn, lowIn);
	findHighLowtemp(vecElement->dataOutside, highOut, lowOut);

	if (highIn > highOut)
	{
		high = highIn; low = lowOut;
	}
	else { high = highOut; low = lowIn; }

	return high - low;
}

//----------------------------------------------------------
// WIP :: Hur l�nge balkongd�rren �r �ppen p� en dag
//----------------------------------------------------------

// R�knar ut hur l�nge d�rren �r �ppen p� en dag
int Rawdata::doorOpen(std::vector <Rawdata *> &vecElement, bool inOut)
{
	int i = 1000;
	return i;
}

//----------------
// :: Bin�r s�kning
//----------------

int Rawdata::binarySearch(std::vector <Rawdata *> &vector, int low, int high, std::string date)
{
	int i_low;
	int i_high;
	if (vector[low / 2 + high / 2]->get_date() == date)
	{
		return low / 2 + high / 2;
	}
	if (vector[low / 2 + high / 2]->get_date() < date)
	{
		i_low = low / 2 + high / 2;
		return binarySearch(vector, i_low, high, date);
	}
	else
	{
		i_high = low / 2 + high / 2;
		return binarySearch(vector, low, i_high, date);
	}
	return 0;
}

void Rawdata::searchDate(std::vector <Rawdata *> &vector, std::string date, bool inOut)
{
	unsigned int vecsize = vector.size();
	int n;
	
	n = binarySearch(vector, 0, vecsize, date);

	if (inOut)
	{
		std::cout << "--------------------\n"
			<< " Date : " << vector[n]->get_date() << "\n"
			<< " Average temperature : " << vector[n]->analyzedInside.get_aveTemperature() << "\n"
			<< " Average humidity : " << vector[n]->analyzedInside.get_aveHumidity() << "\n"
			<< " Mold Index : " << vector[n]->analyzedInside.get_aveMoldIndex() << "\n"
			<< " Temperature difference of in - out : " << vector[n]->get_temperatureDiff() << "\n";
	}
	else
	{
		std::cout << "--------------------\n"
			<< " Date : " << vector[n]->get_date() << "\n"
			<< " Average temperature : " << vector[n]->analyzedOutside.get_aveTemperature() << "\n"
			<< " Average humidity : " << vector[n]->analyzedOutside.get_aveHumidity() << "\n"
			<< " Mold Index : " << vector[n]->analyzedOutside.get_aveMoldIndex() << "\n"
			<< " Temperature difference of in - out : " << vector[n]->get_temperatureDiff() << "\n";
	}
}

//---------------------------------------------
// :: Letar efter meterologisk h�st eller vinter
//---------------------------------------------

// Letar efter meteorologisk h�st och vinter
void Rawdata::SearchSeason(std::vector <Rawdata *> &rawvector, float seasonTempLimit)
{
	unsigned int vecsize = rawvector.size();
	std::string foundDate;

	for (unsigned int i = 0; i < vecsize - 5; i++)
	{
		if (rawvector[i]->analyzedOutside.get_aveTemperature() < seasonTempLimit &&
			rawvector[i + 1]->analyzedOutside.get_aveTemperature() < seasonTempLimit &&
			rawvector[i + 2]->analyzedOutside.get_aveTemperature() < seasonTempLimit &&
			rawvector[i + 3]->analyzedOutside.get_aveTemperature() < seasonTempLimit &&
			rawvector[i + 4]->analyzedOutside.get_aveTemperature() < seasonTempLimit)
		{
			foundDate = rawvector[i]->get_date();
			i = vecsize;
		}
	}
	if (foundDate != "")
	{
		std::cout << foundDate << "\n";
	}
	else
	{
		PRINT("Winter is coming...");
	}
}

//-----------------------------------------
// WIP temperature diff och dooropen k�r dom i inside object :: Sortera analyserad data av �nskad sort
//-----------------------------------------

// Funktioner f�r stable sort, v�ljer vad jag ska den ska sortera och i vilken ordning
bool Rawdata::compareFloat(const tempData &a, const tempData &b)
{
	return a.get_floatValue() > b.get_floatValue();
}
bool Rawdata::compareInt(const tempData &a, const tempData &b)
{
	return a.get_intValue() > b.get_intValue();
}

void Rawdata::sortData(std::vector <tempData> &sortVector, std::vector <Rawdata *> &rawVector, int choice, bool inOut)
{
	std::string typeLabel;
	unsigned int vecsize;
	vecsize = rawVector.size();
	for (unsigned int i = 0; i < vecsize; i++)
	{
		if (inOut)
		{
			sortData_choice(sortVector, rawVector[i], rawVector[i]->analyzedInside, choice);
		}
		else
		{
			sortData_choice(sortVector, rawVector[i], rawVector[i]->analyzedOutside, choice);
		}
	}
	switch (choice)
	{
	case e_aveTemperature:
		typeLabel = " Average temperature : ";
		std::stable_sort(sortVector.begin(), sortVector.end(), compareFloat);
		printVectorTop(sortVector, typeLabel, false);
		break;
	case e_aveHumidity:
		typeLabel = " Average humidity : ";
		std::stable_sort(sortVector.begin(), sortVector.end(), compareInt);
		printVectorTop(sortVector, typeLabel, false);
		break;
	case e_aveMoldIndex:
		typeLabel = " Average mold index : ";
		std::stable_sort(sortVector.begin(), sortVector.end(), compareFloat);
		printVectorTop(sortVector, typeLabel, false);
		break;
	case e_temperatureDiff:
		typeLabel = " Temperature difference in-out : ";
		std::stable_sort(sortVector.begin(), sortVector.end(), compareFloat);
		printVectorTop(sortVector, typeLabel, false);
		break;
	case e_doorOpen:
		typeLabel = " How long the door was open : ";
		std::stable_sort(sortVector.begin(), sortVector.end(), compareInt);
		printVectorTop(sortVector, typeLabel, false);
		break;
	}

	vecsize = sortVector.size();
	for (unsigned int i = 0; i < vecsize; i++)
	{
		sortVector.pop_back();
	}
}

void Rawdata::sortData_choice(std::vector <tempData> &sortVector, Rawdata * &vecElement, Analyzeddata &anaObj, int &choice)
{
	tempData obj;
	switch (choice)
	{
	case e_aveTemperature:
		obj = tempData(vecElement->get_date(), anaObj.get_aveTemperature());
		sortVector.push_back(obj);
		break;
	case e_aveHumidity:
		obj = tempData(vecElement->get_date(), anaObj.get_aveHumidity());
		sortVector.push_back(obj);
		break;
	case e_aveMoldIndex:
		obj = tempData(vecElement->get_date(), anaObj.get_aveMoldIndex());
		sortVector.push_back(obj);
		break;
	case e_temperatureDiff: // ta tempdiff bara ifr�n inside object
		obj = tempData(vecElement->get_date(), anaObj.get_aveTemperatureDiff());
		sortVector.push_back(obj);
		break;
	case e_doorOpen: // ta dooropen bara ifr�n inside object
		obj = tempData(vecElement->get_date(), anaObj.get_doorOpen());
		sortVector.push_back(obj);
		break;
	}
}

void Rawdata::printVectorTop(std::vector <tempData> &sortVector, std::string labelType, bool intType)
{
	unsigned int vecsize = sortVector.size();
	std::cout << "----Top 5 Highest" << labelType << "----" << "\n";
	for (unsigned int i = 0; i < 5; i++)
	{
		if (intType)
		{
			std::cout << " Date : " << sortVector[i].get_date() << "\n";
			std::cout << labelType << sortVector[i].get_intValue() << "\n";
		}
		else
		{
			std::cout << " Date : " << sortVector[i].get_date() << "\n";
			std::cout << labelType << sortVector[i].get_floatValue() << "\n";
		}
	}
	std::cout << "----Top 5 lowest" << labelType << "----" << "\n";
	for (unsigned int i = vecsize - 1; i > vecsize - 6; i--)
	{
		if (intType)
		{
			std::cout << " Date : " << sortVector[i].get_date() << "\n";
			std::cout << labelType << sortVector[i].get_intValue() << "\n";
		}
		else
		{
			std::cout << " Date : " << sortVector[i].get_date() << "\n";
			std::cout << labelType << sortVector[i].get_floatValue() << "\n";
		}
	}
}

Rawdata::~Rawdata()
{
}
