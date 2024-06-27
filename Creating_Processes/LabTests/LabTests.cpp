#include "pch.h"
#include "CppUnitTest.h"
#include "../Employee_functions.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace LabTests
{
	TEST_CLASS(LabTests)
	{
	public:
		
	public:

		TEST_METHOD(testOutputFileBin)
		{
			std::ofstream out("output.bin", std::ios::binary);
			employee emp(1, "John_Doe", 40);
			emp.output_file_bin(out);
			out.close();

			std::ifstream in("output.bin", std::ios::binary);
			int num;
			int length;
			char name[100];
			double hours;

			in.read(reinterpret_cast<char*>(&num), sizeof(num));
			in.read(reinterpret_cast<char*>(&length), sizeof(length));
			in.read(name, length);
			in.read(reinterpret_cast<char*>(&hours), sizeof(hours));

			std::string nameStr(name, length);

			Assert::AreEqual(1, num);
			Assert::AreEqual(std::string("John_Doe"), nameStr);
			Assert::AreEqual(40,0, hours);
		}

		TEST_METHOD(testInputOperator)
		{
			std::istringstream in("1 John_Doe 40.5");
			employee emp;
			in >> emp;

			Assert::AreEqual(1, emp.num);
			Assert::AreEqual(std::string("John_Doe"), emp.name);
			Assert::AreEqual(40.5, emp.hours);
		}

		TEST_METHOD(testOutputOperator)
		{
			std::ostringstream out;
			employee emp(1, "John_Doe", 40);
			out << emp;

			std::string expected = "1 John_Doe 40 ";
			Assert::AreEqual(expected, out.str());
		}

		TEST_METHOD(testAssignmentOperator)
		{
			employee emp1(1, "John Doe", 40.5);
			employee emp2 = emp1;

			Assert::AreEqual(emp1.num, emp2.num);
			Assert::AreEqual(emp1.name, emp2.name);
			Assert::AreEqual(emp1.hours, emp2.hours);
		}

		TEST_METHOD(testCompareEmployee)
		{
			employee emp1(1, "John Doe", 40.5);
			employee emp2(2, "Jane Smith", 35.5);

			bool result1 = compare_employee(emp1, emp2);
			bool result2 = compare_employee(emp2, emp1);

			Assert::IsTrue(result1);
			Assert::IsTrue(!result2);
		}
	};
}
