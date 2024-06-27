#include <iostream>
#include <fstream>
#include "../Employee.h"
#include <string>
#include <string.h>
#include <Windows.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>

#pragma warning (disable:4996)

int main()
{
	int emount = 0;
	std::cout << "input number of elements: ";
	std::cin >> emount;
	std::string file_name;
	std::cout << "input name of binary file (it will be created): ";
	std::cin >> file_name;


	std::string CreatorAllocation("Creator.exe");

	std::string args;
	args += CreatorAllocation + " ";
	args += file_name + " ";
	args += std::to_string(emount);

	STARTUPINFO Creator_StartInf;
	PROCESS_INFORMATION Creator_PrInf;

	ZeroMemory(&Creator_StartInf, sizeof(STARTUPINFO));
	Creator_StartInf.cb = sizeof(STARTUPINFO);
	if (!CreateProcess(nullptr, wcsdup((std::wstring(args.begin(), args.end())).c_str()), nullptr, nullptr, FALSE,
		CREATE_NEW_CONSOLE, nullptr, nullptr, &Creator_StartInf, &Creator_PrInf))
	{
		std::cout << "Creator isn't runned. Stopping program..." << std::endl;
		return 0;
	}
	WaitForSingleObject(Creator_PrInf.hProcess, INFINITE);
	CloseHandle(Creator_PrInf.hThread);
	CloseHandle(Creator_PrInf.hProcess);

	std::ifstream first_check(file_name);
	employee inputter;

	first_check.read(reinterpret_cast<char*>(&emount), sizeof(emount));
	for (size_t i = 0; i < emount; i++)
	{
		inputter.input_file_bin(first_check);
		std::cout << inputter;
		printf("\n");
	}

	std::string report_name;
	std::cout << "input name of report file (it will be created): ";
	std::cin >> report_name;

	double PayForHour = 0;
	std::cout << "input pay for hour: ";
	std::cin >> PayForHour;

	std::string ReporterAllocation("Reporter.exe");

	std::string args_rep;
	args_rep += ReporterAllocation + " ";
	args_rep += file_name + " ";
	args_rep += report_name + " ";
	args_rep += std::to_string(PayForHour);


	STARTUPINFO Reporter_StartInf;
	PROCESS_INFORMATION Reporter_PrInf;

	ZeroMemory(&Reporter_StartInf, sizeof(STARTUPINFO));
	Reporter_StartInf.cb = sizeof(STARTUPINFO);

	if (!CreateProcess(nullptr, wcsdup((std::wstring(args_rep.begin(), args_rep.end())).c_str()), nullptr, nullptr, FALSE,
		CREATE_NEW_CONSOLE, nullptr, nullptr, &Reporter_StartInf, &Reporter_PrInf))
	{
		std::cout << "Reporter isn't runned. Stopping program..." << std::endl;
		return 0;
	}
	WaitForSingleObject(Reporter_PrInf.hProcess, INFINITE);
	CloseHandle(Reporter_PrInf.hThread);
	CloseHandle(Reporter_PrInf.hProcess);

	std::ifstream second_check(report_name);
	employee inputter_rep;

	for (size_t i = 0; i < emount; i++)
	{
		second_check >> inputter_rep;
		double salary = 0;
		second_check >> salary;

		std::cout << inputter_rep;
		std::cout << salary;
		std::cout << std::endl;
	}


	return 0;
}

