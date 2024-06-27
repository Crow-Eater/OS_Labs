#include "Employee.h"
#include <string>
#include <string.h>
#include <iostream>
#include <fstream>

employee::employee() : num(0), hours(0) {}

employee::employee(int new_num, std::string const& new_name, double new_hours) : num(new_num), name(new_name), hours(new_hours) {}

std::ifstream& employee::input_file_bin(std::ifstream& in) 
{
	in.read(reinterpret_cast<char*>(&num), sizeof(num));

	int length = 0;
	in.read(reinterpret_cast<char*>(&length), sizeof(int));
	name = std::string();
	char in_char;
	for (int i = 0; i < length; i++)
	{
		in.read(&in_char, sizeof(in_char));
		name.push_back(in_char);
	}
	in.read(reinterpret_cast<char*>(&hours), sizeof(hours));

	return in;
}

std::ofstream& employee::output_file_bin(std::ofstream& out)
{
	out.write(reinterpret_cast<char*>(&num), sizeof(num));

	int length = name.length();
	out.write(reinterpret_cast<char*>(&length), sizeof(int));
	for(int i = 0; i < name.length(); i++)
	{
		out.write(&name[i], sizeof(name[i]));
	}
	out.write(reinterpret_cast<char*>(&hours), sizeof(hours));

	return out;
}

std::istream& operator>>(std::istream& in, employee &emp)
{
	in >> emp.num;
	in >> emp.name;
	in >> emp.hours;
	return in;
}

std::ostream& operator << (std::ostream& out, employee & emp)
{
	out << emp.num << " ";
	out << emp.name << " ";
	out << emp.hours << " ";
	return out;
}

employee& employee::operator = (employee const& sourse)
{
	if (this == &sourse)
	{
		return *this;
	}
	else
	{
		this->num = sourse.num;
		this->hours = sourse.hours;
		this->name = sourse.name;
		return *this;
	}
}

//comparator for qsort
bool compare_employee(employee const& emp1, employee const& emp2)
{
	return emp1.num < emp2.num;
}


