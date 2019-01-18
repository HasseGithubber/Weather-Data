#include "pch.h"
#include "Rawdata.h"

/*The constructor takes in everything sent as strings. The date is sent directly to its variable as a string, everything else is turned into either integers or floats
when sending it a new instance of the Rawday class. Time is divided into hours, minutes and seconds that later in the Rawday constructor gets calculated into total seconds.
The inOut string says wether the line it was on was holding inside or outside data. Temperature and humidity is just that*/
Rawdata::Rawdata(std::string date, std::string timeH, std::string timeM, std::string timeS, std::string inOut, std::string temperature, std::string humidity)
	: date(date)
{
	/*As the strings hold information for an entire day I chose to use a stringstream method to take out one word in each string,
	that are seperated with a "space", just like I store the data in the strings in order of appereance they come out in order and sent to a new instance
	for a element in my vectors*/
	std::string s_timeH, s_timeM, s_timeS, s_inOut, s_temperature, s_humidity;
	std::stringstream ss_inOut(inOut), ss_timeH(timeH), ss_timeM(timeM), ss_timeS(timeS), ss_temperature(temperature), ss_humidity(humidity);

	/*This says that while a word can be inserted from the stringstream "inOut" into a string variable we continue to find the next word in the other stringstream
	that belong together and push back it into either the inside or outside vector*/
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

//--------------------------------------
// :: File handling and data calculations
//--------------------------------------

/*This method reads all the data from a file and stores it into strings as long as there is a new line in the file.
It first starts off with doing a getline of the first date in the file, then continues with the same process with the
time, divided into hour, minute and second, and place the data is from, the temperature and humidity. After that it checks
if we have jumped to a different date, and this is there to make sure we only mass collect the data that belongs to one date.
Afterwards we collect and append each new getline into our main string variables seperated with a "space"(explained in the Rawdata constructer),
and in the end gets the date of the next line and checks if we haven't gotten into a new day, if that's the case we complete our collection,
pushback our strings and continue until there is no data left.*/
void Rawdata::fileInput(std::vector <Rawdata *> &rawvector)
{
	std::string date, date2, timeH_holder, timeM_holder, timeS_holder, inOut_holder, temperature_holder, humidity_holder, 
		timeH, timeM, timeS, inOut, temperature, humidity;
	bool nextdate = true, endof = false;

	std::ifstream file("tempdata4.csv");
	if (file.is_open())
	{
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
				getline(file, date2, ' ');
			}
			else
			{
				getline(file, date, ' ');
			}
			while (date == date2 && !endof)
			{
				if (!file.eof())
				{
					getline(file, timeH_holder, ':');
					getline(file, timeM_holder, ':');
					getline(file, timeS_holder, ',');

					getline(file, inOut_holder, ',');
					getline(file, temperature_holder, ',');
					getline(file, humidity_holder);

					timeH.append(" ");
					timeH.append(timeH_holder);
					timeM.append(" ");
					timeM.append(timeM_holder);
					timeS.append(" ");
					timeS.append(timeS_holder);
					inOut.append(" ");
					inOut.append(inOut_holder);
					temperature.append(" ");
					temperature.append(temperature_holder);
					humidity.append(" ");
					humidity.append(humidity_holder);

					if (!file.eof())
					{
						if (nextdate)
						{
							getline(file, date2, ' ');
						}
						else
						{
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
				rawvector.push_back(new Rawdata(date2, timeH, timeM, timeS, inOut, temperature, humidity));
				nextdate = true;
			}
		}
	}
	file.close();
}

/*This method is where the raw data is calculated and reinitilized into the rawData vector elements analyzed object.
I chose to mainly send data through the other methods by reference because of keeping the reinitialization cleaner and faster processing.*/
void Rawdata::convertData(std::vector <Rawdata *> &rawvector, std::vector <tempData *> &bDoorVector)
{
	float aveTemperature;
	int aveHumidity, moldRiskTime, doorOpen_hours;
	double aveMoldIndexTime, aveMoldIndex;

	unsigned int vecsize = rawvector.size();

	for (unsigned int i = 0; i < vecsize; i++)
	{
		averageTempHumid(rawvector[i], aveTemperature, aveHumidity, true);
		aveMold_Index(rawvector[i]->dataInside, aveMoldIndex);
		moldRisk_time(rawvector[i]->dataInside, moldRiskTime, aveMoldIndexTime);
		doorOpen(bDoorVector, rawvector[i], doorOpen_hours); // doorOpen data stored only in the the analyzedInside object

		rawvector[i]->analyzedInside = Analyzeddata(aveTemperature, aveHumidity, aveMoldIndex, moldRiskTime, aveMoldIndexTime, temperatureDiffInOut(rawvector[i]), doorOpen_hours);

		averageTempHumid(rawvector[i], aveTemperature, aveHumidity, false);
		aveMold_Index(rawvector[i]->dataOutside, aveMoldIndex);
		moldRisk_time(rawvector[i]->dataOutside, moldRiskTime, aveMoldIndexTime);

		rawvector[i]->analyzedOutside = Analyzeddata(aveTemperature, aveHumidity, aveMoldIndex, moldRiskTime, aveMoldIndexTime, 0, 0);
	}
}

//-------------------------------------------------
// :: Average values of the temperature and humidity
//-------------------------------------------------

/*Calculates the average temperature and humidity of a day from either the inside or outside.*/
void Rawdata::averageTempHumid(Rawdata * &vecElement, float &aveTemp, int &aveHumid, bool inOut)
{
	unsigned int vecsize;

	if (inOut) { vecsize = vecElement->dataInside.size(); }
	else { vecsize = vecElement->dataOutside.size(); };

	aveTemp = 0;
	aveHumid = 0;
	for (unsigned int i = 0; i < vecsize; i++)
	{
		if (inOut)
		{
			aveTemp = aveTemp + vecElement->dataInside[i]->get_temperature();
			aveHumid = aveHumid + vecElement->dataInside[i]->get_humidity();
		}
		else
		{
			aveTemp = aveTemp + vecElement->dataOutside[i]->get_temperature();
			aveHumid = aveHumid + vecElement->dataOutside[i]->get_humidity();
		}
	}
	aveTemp = aveTemp / vecsize;
	aveHumid = aveHumid / vecsize;
}

//----------------------------------------------------------------------------------------------------------------
// :: Methods for an entire days mold index, and how long it had a risk of mold and it's mold index for that period
//----------------------------------------------------------------------------------------------------------------

/*Counts the mold index for each time of one day and takes the average index amount from that*/
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

/*This checks the amount of time it was moldrisk(total) during one day, also takes the average mold index of that period.
It goes through each time period and when it finds a time when it is mold risk it saves that time as a starting time,
then it keeps going through, all the while calculating the times average mold index, and looks for a time when there is no mold risk.
It then saves that time as a stop time and then check the difference between them and adds it to the total mold time of the day.
If a starting time has been found but no stop time it takes the last existing time in the vector and counts from that.*/
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
			n++;
			if (getstarttime)
			{
				startTime = vector[i]->get_time();
				getstarttime = false;
			}
			if (i < vecsize - 2)
			{
				temperature = vector[i + 1]->get_temperature();
				humidMoldThreshold = -0.0015 * pow(temperature, 3) + 0.1193 * pow(temperature, 2) - 2.9878 * temperature + 102.96;
				if (vector[i + 1]->get_humidity() < humidMoldThreshold)
				{
					stopTime = vector[i + 1]->get_time();
					moldRiskTime = moldRiskTime + (stopTime - startTime);
					getstarttime = true;
				}
			}
			else
			{
				stopTime = vector[i + 1]->get_time();
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
// Older code **only here for my own keep sake** :: Största temperatur skillnaden för ute och inne vardera på en dag
//-----------------------------------------------------------------------------
// Räknar ut största temperatur skillnaden på inne och ute temperaturen på en dag
//float Rawdata::temperatureDifferenceSep(Rawdata * &vecElement, bool inOut)
//{
//	unsigned int vecsize;
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
//		for (unsigned int i = 2; i < vecsize; i++)
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
//		for (unsigned int i = 1; i < vecsize; i++)
//		{
//			if (vecElement->dataOutside[i]->get_temperature() < low) { low = vecElement->dataOutside[i]->get_temperature(); }
//			if (vecElement->dataOutside[i]->get_temperature() > high) { high = vecElement->dataOutside[i]->get_temperature(); }
//		}
//	}
//	return high - low;
//}

//-------------------------------------------------------------------------------------
// :: Finds the largest difference of the inside and outside difference total from a day
//-------------------------------------------------------------------------------------

// **Reminder to myself** "abs(value - value)" gives you the absolute difference.

/*This one starts with variables that gets sent into it's own method depending if its inside or outside
to find the highest and lowest temperature of that day. When it's found it compares the variables so that the
highest one is always subtracted by the lowest. I later found out about the abs() function which could probably be implemented.*/
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

/*A method that goes through a Rawday vector and finds the highest and lowest temperature.
It starts with the first and compares that with the second one, the higher one gets put into the high variable
and the other one into low. Next it goes through each element an replaces the high, low variables with the new value
if its lower or higher.*/
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

//----------------------------------------------------------------
// :: How long the balcony door was open during a day, in hours
//----------------------------------------------------------------


/*Algorithm that calculates how long the balcony door in the room has been open, in hours.
-- Found a fault in the algorithm, the first hour never gets represented.--*/
void Rawdata::doorOpen(std::vector <tempData *> &bDoorVector, Rawdata * &rawVecElement, int &doorOpenHours)
{
	float diffTemp, diffHumid;
	rawVecElement->hourlyAverageTemp(bDoorVector, rawVecElement->dataInside, rawVecElement->dataOutside);

	doorOpenHours = 0;
	for (int i = 1; i < 24; i++)
	{
		if (bDoorVector[i - 1]->get_tempInside() != 0 || bDoorVector[i - 1]->get_tempOutside() != 0
			&& bDoorVector[i]->get_tempInside() != 0 || bDoorVector[i]->get_tempOutside() != 0)
		{
			if(bDoorVector[i - 1]->get_tempDifference() > bDoorVector[i]->get_tempDifference()
				&& bDoorVector[i - 1]->get_humidDifference() > bDoorVector[i]->get_humidDifference())
			{
				diffTemp = abs(bDoorVector[i - 1]->get_tempInside() - bDoorVector[i]->get_tempInside());
				diffHumid = abs(bDoorVector[i - 1]->get_humidInside() - bDoorVector[i]->get_humidInside());

				if (diffTemp > 0.2 && bDoorVector[i - 1]->get_tempInside() < bDoorVector[i]->get_tempInside()
					&& diffHumid > 0.2 && bDoorVector[i - 1]->get_humidInside() - bDoorVector[i]->get_humidInside())
				{
					doorOpenHours++;
				}
			}
		}
	}
}

/* */
void Rawdata::hourlyAverageTemp(std::vector <tempData *> &bDoorVector, std::vector <Rawday *> &insideVec, std::vector <Rawday *> &outsideVec)
{
	int vecsizeIn = insideVec.size();
	int vecsizeOut = outsideVec.size();
	float insideTemp = 0, outsideTemp = 0, tempDifference = 0;
	float insideHumid = 0, outsideHumid = 0, humidDifference = 0;

	int hour_counter = 1, i = 0, j = 0, n = 0;
	do
	{
		if (i < vecsizeIn)
		{
			while (i < vecsizeIn && insideVec[i]->get_time() < hour_counter * 3600)
			{
				insideTemp = insideTemp + insideVec[i]->get_temperature();
				insideHumid = insideHumid + insideVec[i]->get_humidity();
				i++;
				n++;
			}
			if (n != 0)
			{
				insideTemp = insideTemp / n;
				insideHumid = insideHumid / n;
			}
		}
		n = 0;
		if (j < vecsizeOut)
		{
			while (j < vecsizeOut && outsideVec[j]->get_time() < hour_counter * 3600)
			{
				outsideTemp = outsideTemp + outsideVec[j]->get_temperature();
				outsideHumid = outsideHumid + outsideVec[j]->get_humidity();
				j++;
				n++;
			}
			if (n != 0)
			{
				outsideTemp = outsideTemp / n;
				outsideHumid = outsideHumid / n;
			}
		}
		n = 0;
		if (insideTemp + outsideTemp != 0)
		{
			tempDifference = abs(insideTemp - outsideTemp);
		}
		if (insideHumid + outsideHumid != 0)
		{
			humidDifference = abs(insideHumid - outsideHumid);
		}

		bDoorVector.push_back(new tempData(insideTemp, outsideTemp, tempDifference, insideHumid, outsideHumid, humidDifference));

		hour_counter++;
	} while (bDoorVector.size() < 24);
}

//------------------------------------------------
// :: A binary search function to find a given date
//------------------------------------------------

/*The start of the search that when a match has been found prints the corresponding data*/
void Rawdata::searchDate(std::vector <Rawdata *> &vector, std::string date, bool inOut)
{
	unsigned int vecsize = vector.size();
	int n;

	n = binarySearch(vector, 0, vecsize, date);
	if (n != -1)
	{
		if (inOut)
		{
			std::cout << "--------------------\n"
				<< " Date : " << vector[n]->get_date() << "\n"
				<< " Average temperature : " << vector[n]->analyzedInside.get_aveTemperature() << "\n"
				<< " Average humidity : " << vector[n]->analyzedInside.get_aveHumidity() << "\n"
				<< " Mold Index : " << vector[n]->analyzedInside.get_aveMoldIndex() << "\n"
				<< " -------------\n"
				<< " Moldrisk total time : " << vector[n]->analyzedInside.get_moldriskTime() << "\n"
				<< " Mold Index for that time : " << vector[n]->analyzedInside.get_aveMoldIndTime() << "\n";
		}
		else
		{
			std::cout << "--------------------\n"
				<< " Date : " << vector[n]->get_date() << "\n"
				<< " Average temperature : " << vector[n]->analyzedOutside.get_aveTemperature() << "\n"
				<< " Average humidity : " << vector[n]->analyzedOutside.get_aveHumidity() << "\n"
				<< " Mold Index : " << vector[n]->analyzedOutside.get_aveMoldIndex() << "\n"
				<< " -------------\n"
				<< " Moldrisk total time : " << vector[n]->analyzedOutside.get_moldriskTime() << "\n"
				<< " Mold Index for that time : " << vector[n]->analyzedOutside.get_aveMoldIndTime() << "\n";
		}
	}
	else
	{
		std::cout << "Date not found" << "\n";
	}
}

/*The binary search does two things, it takes the middle place of a given part and checks if the search value
is what we are looking for or if not if it's higher or lower than that. If not it takes the half where it should be and
repeats the process until the right spot has been found. It's a quick way to search for a value in a sorted array.*/
int Rawdata::binarySearch(std::vector <Rawdata *> &vector, int low, int high, std::string date)
{
	int i_low;
	int i_high;
	if (high > low)
	{
		if (vector[low / 2 + high / 2]->get_date() == date)
		{
			return low / 2 + high / 2;
		}
		if (vector[low / 2 + high / 2]->get_date() < date)
		{
			i_low = low / 2 + high / 2;
			return binarySearch(vector, i_low + 1, high, date);
		}
		else
		{
			i_high = low / 2 + high / 2;
			return binarySearch(vector, low, i_high - 1, date);
		}
	}
	return -1;
}


//-------------------------------------------------------------------------------------
// :: Searches for the meteorological start of a season, currently just autumn or winter
//-------------------------------------------------------------------------------------

/*Searches through the raw vectors average temperatures and looks for when 5 days in a row are all below 10
or 0, when found it stops and prints the date belonging to the index that was at the index i.*/
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
// WIP temperature diff och dooropen kör dom i inside object :: Sorts the wanted data and prints the top 5 highest and lowest
//-----------------------------------------

/*Functions which are used in the stable sort command, it tells the sorter what to sort and in what order*/
bool Rawdata::compareFloat(const tempData &a, const tempData &b)
{
	return a.get_floatValue() > b.get_floatValue();
}
bool Rawdata::compareInt(const tempData &a, const tempData &b)
{
	return a.get_intValue() > b.get_intValue();
}

/*Takes in a vector to store data temporarly and the rawdata vector, an integer that dictates what to sort
and a boolean to let the method know which in or out vector to get data from. When the data has been collected
it sorts the temporary vector using the stable sort which is a stable version of a mergesort and sends it to 
the print method. When that is done it pop backs the vector deleting all the elements restoring it to zero size. */
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
		typeLabel = " Door was open, in hours : ";
		std::stable_sort(sortVector.begin(), sortVector.end(), compareInt);
		printVectorTop(sortVector, typeLabel, true);
		break;
	}

	vecsize = sortVector.size();
	for (unsigned int i = 0; i < vecsize; i++)
	{
		sortVector.pop_back();
	}
}

/*This method lets program know what kind of data we want to store in the temporary vector.*/
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
	case e_temperatureDiff: // Temperture diff is only stored in the Analyzed object containing inside data
		obj = tempData(vecElement->get_date(), anaObj.get_aveTemperatureDiff());
		sortVector.push_back(obj);
		break;
	case e_doorOpen: // Door open time is only stored in the Analyzed object containing inside data
		obj = tempData(vecElement->get_date(), anaObj.get_doorOpen());
		sortVector.push_back(obj);
		break;
	}
}

/*Prints top 5 highest and lowest values*/
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

/*Makes sure there is no memory leak*/
void Rawdata::deleteVectors(std::vector <Rawdata *> &rawVector)
{
	int vecsize, invecsize, outvecsize;
	vecsize = rawVector.size();
	for (int i = 0; i < vecsize; i++)
	{
		invecsize = rawVector[i]->dataInside.size();
		outvecsize = rawVector[i]->dataOutside.size();
		for (int x = 0; x < invecsize; x++)
		{
			delete rawVector[i]->dataInside[x];
		}
		for (int y = 0; y < outvecsize; y++)
		{
			delete rawVector[i]->dataOutside[y];
		}
	}
}

Rawdata::~Rawdata()
{

}
