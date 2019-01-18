#include "pch.h"
#include "Program.h"


Program::Program()
{
}

/*The starting method to run the Rawdata class methods used for taking in all the data from the file and convert
it into useful information*/
void Program::loadProcessData()
{
	std::cout << " Loading in raw data" << std::endl;
	access.fileInput(rawData);
	std::cout << " Analyzing data" << std::endl;
	access.convertData(rawData, balconyDoorVector);
	mainmenu();
}

/*The mainmenu, takes you to inside and outside menus and shows the top 5 highest and lowest temperature differences
and how long the balcony door was open in hours for each day*/
void Program::mainmenu()
{
	system("cls");
	bool stay = true;
	int vecsize, vecsizebd;
	do
	{
		std::cout << " -Weather Analyzer-" << std::endl;
		std::cout << " [1] Inspect inside data" << std::endl;
		std::cout << " [2] Inspect outside data" << std::endl;
		std::cout << " [3] Show the top 5 temperature differences - high to low" << std::endl;
		std::cout << " [4] How long the balcony door was open per day - high to low" << std::endl;
		std::cout << " [5] Exit program" << std::endl;
		std::cin >> answer;
		switch (answer[0])
		{
		case '1': // Inside data menu
			stay = false;
			break;
		case '2': // Outside data menu
			stay = false;
			break;
		case '3':
			system("cls");
			access.sortData(sortedVector, rawData, 3, true);
			break;
		case '4':
			system("cls");
			access.sortData(sortedVector, rawData, 4, true);
			break;
		case '5':
			access.deleteVectors(rawData);
			vecsize = rawData.size();
			vecsizebd = balconyDoorVector.size();
			for (int i = 0; i < vecsize; i++)
			{
				delete rawData[i];
			}
			for (int i = 0; i < vecsizebd; i++)
			{
				delete balconyDoorVector[i];
			}
			stay = false;
			break;
		default:
			std::cout << "Wrong input" << std::endl;
			break;
		}
	} while (stay);
	switch (answer[0])
	{
	case '1':
		insideOptions();
		break;
	case '2':
		outsideOptions();
		break;
	}
}

/*The inside menu, here you can search for a date and it will show it's corresponding data,
other options are top 5 highest and lowest of a datatype you want to see*/
void Program::insideOptions()
{
	system("cls");
	std::string date;
	int i;
	bool stay = true;
	do
	{
		std::cout << " -Inside information-" << std::endl;
		std::cout << " [1] Search for a date" << std::endl;
		std::cout << " [2] Show the top 5 average temperatures - high to low" << std::endl;
		std::cout << " [3] Show the top 5 average humidity levels - dry to humid" << std::endl;
		std::cout << " [4] Show the top 5 mold risk levels - high to low" << std::endl;
		std::cout << " [5] Back to main menu" << std::endl;
		std::cin >> answer;
		switch (answer[0])
		{
		case '1':
			system("cls");
			do
			{
				std::cout << " Enter a date \"e.g 2016-10-04\" : ";
				std::cin >> answer;
				i = answer.size();
				if (i != 10)
				{
					std::cout << "Answer needs to look like the example" << "\n";
				}
			} while (i != 10);
			access.searchDate(rawData, answer, true);
			break;
		case '2':
			system("cls");
			access.sortData(sortedVector, rawData, 0, true);
			break;
		case '3':
			system("cls");
			access.sortData(sortedVector, rawData, 1, true);
			break;
		case '4':
			system("cls");
			access.sortData(sortedVector, rawData, 2, true);
			break;
		case '5':
			stay = false;
			break;
		default:
			std::cout << "Wrong input" << std::endl;
			break;
		}
	} while (stay);
	if (answer[0] == '5')
	{
		mainmenu();
	}
}

/*Much like the inside options but here you can also go to a search menu to get the date when a season started*/
void Program::outsideOptions()
{
	system("cls");
	std::string date;
	int i;
	bool stay = true;
	do
	{
		std::cout << " -Outside information-" << std::endl;
		std::cout << " [1] Search for a date" << std::endl;
		std::cout << " [2] Show the top 5 average temperatures - high to low" << std::endl;
		std::cout << " [3] Show the top 5 average humidity levels - dry to humid" << std::endl;
		std::cout << " [4] Show the top 5 mold risk levels - high to low" << std::endl;
		std::cout << " [5] Search for the beginning of a season" << std::endl;
		std::cout << " [6] Back to main menu" << std::endl;
		std::cin >> answer;
		switch (answer[0])
		{
		case '1':
			system("cls");
			do
			{
				std::cout << " Enter a date \"e.g 2016-10-04\" : ";
				std::cin >> answer;
				i = answer.size();
				if (i != 10)
				{
					std::cout << "Answer needs to look like the example" << "\n";
				}
			} while (i != 10);
			access.searchDate(rawData, answer, false);
			break;
		case '2':
			system("cls");
			access.sortData(sortedVector, rawData, 0, false);
			break;
		case '3':
			system("cls");
			access.sortData(sortedVector, rawData, 1, false);
			break;
		case '4':
			system("cls");
			access.sortData(sortedVector, rawData, 2, false);
			break;
		case '5':
			stay = false;
			break;
		case '6':
			stay = false;
			break;
		default:
			std::cout << "Wrong input" << std::endl;
			break;
		}
	} while (stay);
	switch (answer[0])
	{
	case '5':
		searchSeason();
		break;
	case '6':
		mainmenu();
		break;
	}
}

/*Menu where the user can choose what season they want to search for and prints the date
for when it starts, if it does. Currently, with the given data, only finds a starting date for autumn.*/
void Program::searchSeason()
{
	system("cls");
	bool stay = true;
	do
	{
		std::cout << " -Search for the beginning of a season-" << std::endl;
		std::cout << " [1] Spring" << std::endl;
		std::cout << " [2] Summer" << std::endl;
		std::cout << " [3] Autumn" << std::endl;
		std::cout << " [4] Winter" << std::endl;
		std::cout << " [5] Back to main menu" << std::endl;
		std::cin >> answer;
		switch (answer[0])
		{
		case '1':
			system("cls");
			PRINT("We haven't even had spring yet...");
			break;
		case '2':
			system("cls");
			PRINT("We haven't even had winter nor spring yet...");
			break;
		case '3':
			system("cls");
			access.SearchSeason(rawData, 10);
			break;
		case '4':
			system("cls");
			access.SearchSeason(rawData, 0);
			break;
		case '5':
			stay = false;
			break;
		default:
			std::cout << "Wrong input" << std::endl;
			break;
		}
	} while (stay);
	if (answer[0] == '5')
	{
		mainmenu();
	}
}

Program::~Program()
{
}
