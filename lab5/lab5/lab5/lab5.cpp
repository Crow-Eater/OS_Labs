#include <fstream>
#include <vector>
#include <windows.h>
#include <iostream>
#include <conio.h>

using std::cin;
using std::cout;
using std::string;
using std::wstring;
using std::ofstream;
using std::ifstream;

struct employee
{
	int num;
	char name[10];
	double hours;
};

string file_name;
int amount_of_employees;
employee* employees;
int amount_of_clients;
HANDLE* hPipes;
HANDLE* hThreads;
HANDLE* hSemaphores;
HANDLE* hProcesses;

DWORD WINAPI ServerOperations(LPVOID pipe)
{
	HANDLE hPipe = (HANDLE)pipe;
	DWORD bytesRead = 0;
	DWORD bytesWrite = 0;

	int message;
	while (true)
	{
		ReadFile(hPipe, &message, sizeof(message), &bytesRead, NULL);

		int ID = message / 10;
		int option = message % 10;
		if (option == 1)
		{
			for (int i = 0; i < amount_of_clients; i++)
			{
				WaitForSingleObject(hSemaphores[ID - 1], INFINITE);
			}

			employee* emp = new employee;
			*emp = employees[ID - 1];
			bool written = WriteFile(hPipe, emp, sizeof(employee), &bytesWrite, NULL);
			if (written)
			{
				cout << "������ ����������\n";
			}
			else
			{
				cout << "������ �� ���� ����������\n";
			}

			bool read = ReadFile(hPipe, emp, sizeof(employee), &bytesWrite, NULL);
			if (!read)
			{
				cout << "���������� �������� ������\n";
			}
			employees[ID - 1] = *emp;

			ofstream fout(file_name);
			for (int i = 0; i < amount_of_employees; i++)
			{
				fout << employees[i].num
					<< " " << employees[i].name
					<< " " << employees[i].hours << "\n";
			}
			fout.close();

			int msg;
			read = ReadFile(hPipe, &msg, sizeof(msg), &bytesWrite, NULL);
			if (!read)
			{
				std::cout << "���������� �������� ������\n";
			}

			if (msg == 1)
			{
				for (int i = 0; i < amount_of_clients; i++)
				{
					ReleaseSemaphore(hSemaphores[ID - 1], 1, NULL);
				}
			}
		}
		if (option == 2)
		{
			WaitForSingleObject(hSemaphores[ID - 1], INFINITE);

			employee* emp = new employee();
			*emp = employees[ID - 1];
			bool written = WriteFile(hPipe, emp, sizeof(employee), &bytesWrite, NULL);
			if (written)
			{
				cout << "������ ����������\n";
			}
			else
			{
				cout << "������ �� ���� ����������\n";
			}

			int msg;
			bool read = ReadFile(hPipe, &msg, sizeof(msg), &bytesWrite, NULL);
			if (!read)
			{
				cout << "���������� �������� ������\n";
			}

			if (msg == 1)
			{
				ReleaseSemaphore(hSemaphores[ID - 1], 1, NULL);
			}
		}
		if (option == 3)
		{
			break;
		}

	}

	DisconnectNamedPipe(hPipe);
	CloseHandle(hPipe);

	return 0;
}

int main()
{
	setlocale(LC_ALL, "ru");

	cout << "������� ��� �����: ";
	cin >> file_name;

	cout << "������� ���-�� �����������: ";
	cin >> amount_of_employees;

	employees = new employee[amount_of_employees];
	for (int i = 0; i < amount_of_employees; i++)
	{
		cout << "\n������� ���������� � ���������� " << i + 1 << "\n\n";

		employees[i].num = i + 1;

		cout << "������� ���: ";
		cin >> employees[i].name;

		cout << "������� ���������� ������������ �����: ";
		cin >> employees[i].hours;
	}

	ofstream fout(file_name);
	for (int i = 0; i < amount_of_employees; i++)
	{
		fout << employees[i].num << " " << employees[i].name << " " << employees[i].hours << "\n";
	}
	fout.close();

	cout << "\n����������:\n\n";

	ifstream fin(file_name);
	for (int i = 0; i < amount_of_employees; i++)
	{
		int id;
		string name;
		double hours;
		fin >> id >> name >> hours;

		cout << "ID: " << id << "\t���: " << name << "\t������������ ����: " << hours << "\n\n";
	}
	fin.close();

	cout << "������� ���-�� ��������: ";
	cin >> amount_of_clients;

	hSemaphores = new HANDLE[amount_of_employees];
	for (int i = 0; i < amount_of_employees; i++)
	{
		hSemaphores[i] = CreateSemaphore(NULL, amount_of_clients, amount_of_clients, L"semaphore");
	}

	hProcesses = new HANDLE[amount_of_clients];
	for (int i = 0; i < amount_of_clients; ++i)
	{
		STARTUPINFO si;
		PROCESS_INFORMATION pi;

		string cmd = "Client.exe";
		wstring strToWstr = std::wstring(cmd.begin(), cmd.end());
		LPWSTR clientCmdLine = &strToWstr[0];

		ZeroMemory(&si, sizeof(STARTUPINFO));
		si.cb = sizeof(STARTUPINFO);

		CreateProcess(NULL, clientCmdLine, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
		hProcesses[i] = CreateEvent(NULL, FALSE, FALSE, L"Process Started");

		CloseHandle(pi.hProcess);
	}
	WaitForMultipleObjects(amount_of_clients, hProcesses, TRUE, INFINITE);

	hPipes = new HANDLE[amount_of_clients];
	hThreads = new HANDLE[amount_of_clients];
	for (int i = 0; i < amount_of_clients; i++)
	{
		hPipes[i] = CreateNamedPipe(L"\\\\.\\pipe\\qwe", PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT, PIPE_UNLIMITED_INSTANCES, 0, 0, INFINITE, NULL);
		hThreads[i] = CreateThread(NULL, 0, ServerOperations, static_cast<LPVOID>(hPipes[i]), 0, NULL);
	}
	WaitForMultipleObjects(amount_of_clients, hThreads, TRUE, INFINITE);

	cout << "\n����������:\n\n";

	fin.open(file_name);
	for (int i = 0; i < amount_of_employees; i++)
	{
		int id;
		string name;
		double hours;
		fin >> id >> name >> hours;

		cout << "ID: " << id << "\t���: " << name << "\t������������ ����: " << hours << "\n\n";
	}
	fin.close();

	delete[] hProcesses;
	delete[] hSemaphores;
	delete[] hPipes;
	delete[] hThreads;

	_getch();

	return 0;
}