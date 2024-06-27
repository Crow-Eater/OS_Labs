#pragma once
#include <string>
#include <string.h>
#include <iostream>
#include <fstream>

struct employee
{
	int num; // идентификационный номер сотрудника
	std::string name; // имя сотрудника
	double hours; // количество отработанных часов

	employee();

	employee(int new_num, std::string const& new_name, double new_hours);

	employee(employee const& emp) = default;

	~employee() = default;

	std::ifstream& input_file_bin(std::ifstream& in);

	std::ofstream& output_file_bin(std::ofstream& out);

	friend std::istream& operator >> (std::istream& in, employee & emp);

	friend std::ostream& operator << (std::ostream& out, employee & emp);

	friend std::ifstream& operator >> (std::ifstream& in, employee& emp)
	{
		in >> emp.num;
		in >> emp.name;
		in >> emp.hours;
		return in;
	}

	friend std::ofstream& operator << (std::ofstream& out, employee& emp)
	{
		out << emp.num << " ";
		out << emp.name << " ";
		out << emp.hours << " ";
		return out;
	}

	employee& operator = (employee const& sourse);
};

bool compare_employee(employee const& emp1, employee const& emp2);