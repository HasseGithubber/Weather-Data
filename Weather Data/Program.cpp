#include "pch.h"
#include "Program.h"


Program::Program()
{
}

void Program::loadProcessData()
{
	std::cout << " Loading in raw data" << std::endl;
	access.fileInput(rawData);
	std::cout << " Analyzing data" << std::endl;
	access.convertData(rawData);
	mainmenu();
}

void Program::mainmenu()
{
	bool stay = true;
	std::cout << " -Weather Analyzer-" << std::endl;
	std::cout << " [1] Search for a date" << std::endl;
	std::cout << " [2] Inspect inside data" << std::endl;
	std::cout << " [3] Inspect outside data" << std::endl;
	std::cout << " [4] Show the top 5 temperature differences - high to low" << std::endl;
	std::cout << " [5] How long the balcony door was open per day - high to low" << std::endl;
	std::cout << " [6] Exit program" << std::endl;
	do
	{
		std::cin >> answer;
		switch (answer[0])
		{
		case '1':

			//stay = false;
			break;
		case '2':
			//stay = false;
			break;
		case '3':
			//stay = false;
			outsideOptions();
			break;
		case '4':
			//stay = false;
			break;
		case '5':
			//stay = false;
			break;
		case '6':
			stay = false;
			break;
		default:
			std::cout << "Wrong input" << std::endl;
			break;
		}
	} while (stay);
}

void Program::insideOptions()
{
	bool stay = true;
	std::cout << " -Inside information-" << std::endl;
	std::cout << " [1] Show the top 5 average temperatures - high to low" << std::endl;
	std::cout << " [2] Show the top 5 average humidity levels - dry to humid" << std::endl;
	std::cout << " [3] Show the top 5 mold risk levels - high to low" << std::endl;
	std::cout << " [4] Back to main menu" << std::endl;
	do
	{
		std::cin >> answer;
		switch (answer[0])
		{
		case '1':
			//stay = false;
			break;
		case '2':
			//stay = false;
			break;
		case '3':
			//stay = false;
			break;
		case '4':
			stay = false;
			mainmenu();
			break;
		default:
			std::cout << "Wrong input" << std::endl;
			break;
		}
	} while (stay);
}

void Program::outsideOptions()
{
	bool stay = true;
	std::cout << " -Inside information-" << std::endl;
	std::cout << " [1] Show the top 5 average temperatures - high to low" << std::endl;
	std::cout << " [2] Show the top 5 average humidity levels - dry to humid" << std::endl;
	std::cout << " [3] Show the top 5 mold risk levels - high to low" << std::endl;
	std::cout << " [4] Search for the beginning of a season" << std::endl;
	std::cout << " [5] Back to main menu" << std::endl;
	do
	{
		std::cin >> answer;
		switch (answer[0])
		{
		case '1':
			//stay = false;
			break;
		case '2':
			//stay = false;
			break;
		case '3':
			//stay = false;
			break;
		case '4':
			stay = false;
			searchSeason();
			break;
		case '5':
			stay = false;
			mainmenu();
			break;
		default:
			std::cout << "Wrong input" << std::endl;
			break;
		}
	} while (stay);
}

void Program::searchSeason()
{
	bool stay = true;
	std::cout << " -Search for the beginning of a season-" << std::endl;
	std::cout << " [1] Spring" << std::endl;
	std::cout << " [2] Summer" << std::endl;
	std::cout << " [3] Autumn" << std::endl;
	std::cout << " [4] Winter" << std::endl;
	std::cout << " [5] Back to main menu" << std::endl;
	do
	{
		std::cin >> answer;
		switch (answer[0])
		{
		case '1':
			//stay = false;
			PRINT("We haven't even had spring yet...");
			break;
		case '2':
			//stay = false;
			PRINT("We haven't even had winter nor spring yet...");
			break;
		case '3':
			//stay = false;
			access.SearchSeason(rawData, 10);
			break;
		case '4':
			//stay = false;
			access.SearchSeason(rawData, 0);
			break;
		case '5':
			stay = false;
			mainmenu();
			break;
		default:
			std::cout << "Wrong input" << std::endl;
			break;
		}
	} while (stay);
}

Program::~Program()
{
}
