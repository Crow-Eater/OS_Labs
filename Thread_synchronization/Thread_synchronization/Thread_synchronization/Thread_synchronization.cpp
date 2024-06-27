#include <windows.h>
#include <iostream>
#include <queue>
using std::cout;
using std::cin;

enum class Product { Table, Chair, Bookshelf, Lamp, Door, Sofa };
std::queue<Product> queue_;
	
DWORD WINAPI producer(LPVOID lpParameter)
{
	CONST HANDLE hMutex = (CONST HANDLE)lpParameter;
	srand(time(NULL));
	while (true)
	{
		WaitForSingleObject(hMutex, INFINITE);
		Sleep(5);
		queue_.push(static_cast<Product>(rand() % 6));
		Sleep(5);
		ReleaseMutex(hMutex);
		if (rand() % 10 == 6)
		{
			ExitThread(0);
		}
	}
}


DWORD WINAPI consumer(LPVOID lpParameter)
{
	CONST HANDLE hMutex = (CONST HANDLE)lpParameter;
	srand(time(NULL));
	while (true)
	{
		WaitForSingleObject(hMutex, INFINITE);
		Sleep(7);
		if (queue_.empty())
		{
			ReleaseMutex(hMutex);
			ExitThread(0);
		}

		Product pr = queue_.front();

		switch (pr)
		{
		case Product::Table:
		{
			cout << "Table\n";
			break;
		}
		case Product::Chair:
		{
			cout << "Chair\n";
			break;
		}
		case Product::Bookshelf:
		{
			cout << "Bookshelf\n";
			break;
		}
		case Product::Lamp:
		{
			cout << "Lamp\n";
			break;
		}
		case Product::Door:
		{
			cout << "Door\n";
			break;
		}
		case Product::Sofa:
		{
			cout << "Sofa\n";
			break;
		}
		}
		queue_.pop();
		Sleep(7);
		ReleaseMutex(hMutex);
	}
}


int main()
{
	std::cout << "Enter the number of producers:\n";
	int number_of_producers;
	std::cin >> number_of_producers;

	std::cout << "Enter amount of consumers:\n";
	int number_of_consumers;
	std::cin >> number_of_consumers;


	HANDLE* consumersThreads = new HANDLE[number_of_consumers];

	HANDLE* producersThreads = new HANDLE[number_of_producers];

	CONST HANDLE hMutex = CreateMutex(NULL, FALSE, NULL);


	if (hMutex == NULL)
	{
		cout << "Open mutex failed." << "\n";
		cout << "Press any key to exit." << "\n";
		cin.get();
		return GetLastError();
	}

	for (int i = 0; i < number_of_producers; i++)
	{
		producersThreads[i] = CreateThread(NULL, 0, &producer, hMutex, 0, NULL);
		if (producersThreads[i] == NULL)
			return GetLastError();
		Sleep(100);
	}


	for (int i = 0; i < number_of_consumers; i++)
	{
		consumersThreads[i] = CreateThread(NULL, 0, &consumer, hMutex, 0, NULL);
		if (consumersThreads[i] == NULL)
			return GetLastError();
		Sleep(100);
	}

	WaitForMultipleObjects(number_of_producers, producersThreads, TRUE, INFINITE);
	WaitForMultipleObjects(number_of_consumers, consumersThreads, TRUE, INFINITE);
	CloseHandle(hMutex);

	for (int i = 0; i < number_of_producers; i++)
	{
		CloseHandle(producersThreads[i]);
	}

	for (int i = 0; i < number_of_consumers; i++)
	{
		CloseHandle(consumersThreads[i]);
	}

	return 0;
}