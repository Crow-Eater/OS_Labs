#include <iostream>
#include <thread>
using std::cin;
using std::cout;

int size_numbers = 0;
int* numbers = nullptr;
int min_num = 0;
int max_num = 0;
int average_number = 0;


void min_max()
{
	min_num = numbers[0];
	max_num = numbers[0];

	for (int i = 0; i < size_numbers; i++)
	{
		if (min_num > numbers[i])
		{
			min_num = numbers[i];
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(7));

		if (max_num < numbers[i])
		{
			max_num = numbers[i];
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(7));
	}

	std::cout << "\nMinimal element " << min_num;
	std::cout << "\nMaximal element " << max_num;

	return;
}


void average()
{
	for (int i = 0; i < size_numbers; i++)
	{
		average_number += numbers[i];
		std::this_thread::sleep_for(std::chrono::milliseconds(12));
	}

	average_number /= size_numbers;

	cout << "Average is " << average_number << "\n";

	return;
}


int main()
{

	cout << "Enter the number of elements \n";
	cin >> size_numbers;
	cout << "Enter elements \n";

	numbers = new int[size_numbers];

	for (int i = 0; i < size_numbers; i++)
	{
		cin >> numbers[i];
	}

	std::thread thr1(min_max);
	std::thread thr2(average);

	thr1.join();
	thr2.join();

	for (int i = 0; i < size_numbers; i++)
	{
		if (numbers[i] == min_num)
		{
			numbers[i] = average_number;
		}
	}
	for (int i = 0; i < size_numbers; i++)
	{
		if (numbers[i] == max_num)
		{
			numbers[i] = average_number;
		}
	}

	cout << "\n";

	for (int i = 0; i < size_numbers; i++)
	{
		cout << numbers[i] << " ";
	}


	delete[] numbers;

	return 0;
}