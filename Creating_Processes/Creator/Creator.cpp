#include <iostream>
#include <Windows.h>
#include "../Employee.h"
#include <string>
#include <string.h>
#include <fstream>
#include <vector>

#pragma warning (disable:4996)

int main(int argc, char** argv)
{
	employee inputter;
	std::ofstream file_create(argv[1]);
	int emount = atoi(argv[2]);

	file_create.write(reinterpret_cast<char*>(&emount), sizeof(emount));

	for (size_t i = 0; i < emount; i++)
	{
		std::cout << "Input employee " << i + 1 << std::endl;
		std::cin >> inputter;
		inputter.output_file_bin(file_create);
	}

	return 0;
}
