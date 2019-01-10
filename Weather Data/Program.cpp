#include "pch.h"
#include "Program.h"


Program::Program()
{
}

void Program::mainmenu()
{
	std::cout << " Loading in raw data" << std::endl;

	//Load data
	access.fileInput(rawData);

	std::cout << " Analyzing data" << std::endl;
	//Analyze data
	access.convertData(rawData);

	bool stay = true;
	std::cout << " -Weather Analyzer-" << std::endl;
	std::cout << " [1] Inspect inside data" << std::endl;
	std::cout << " [2] Inspect outside data" << std::endl;
	std::cout << " [3] Exit program" << std::endl;
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
			stay = false;
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
