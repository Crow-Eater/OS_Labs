#include <iostream>
#include <Windows.h>
#include "../Employee.h"
#include <string>
#include <string.h>
#include <fstream>
#include <vector>
#include <algorithm>

#pragma warning (disable:4996)

int main(int argc, char** argv)
{
	std::ifstream binary_input(argv[1]);
	std::ofstream report_out(argv[2]);

	int emount = 0;
	employee inputter;

	binary_input.read(reinterpret_cast<char*>(&emount), sizeof(emount));
	std::vector<employee> myarray(emount, employee());

	for (size_t i = 0; i < emount; i++)
	{
		myarray[i].input_file_bin(binary_input);
	}

	std::sort(myarray.begin(), myarray.end(), compare_employee);

	double PayForHour = atof(argv[3]);
	for (size_t i = 0; i < emount; i++)
	{
		report_out << myarray[i];
		double salary = myarray[i].hours * PayForHour;
		report_out << salary << std::endl;
	}

	return 0;
}
