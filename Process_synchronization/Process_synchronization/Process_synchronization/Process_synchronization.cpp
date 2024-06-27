#include <windows.h>
#include <iostream>
#include "queue.h"
#include <fstream>
#include <string>

using namespace std;
int main()
{
	srand(time(NULL));
	string bin_file_name;
	cout << "input name of file .bin\n";
	cin >> bin_file_name;
	bin_file_name += ".bin";
	ofstream fout;

	DWORD dwWaitResult;
	STARTUPINFO* si = new STARTUPINFO[3];
	PROCESS_INFORMATION* pi = new PROCESS_INFORMATION[3];
	HANDLE hMutex;
	HANDLE hProducerEvent;

	string ProducerEventN = "Process_producer";
	wstring transformed_producer_event = wstring(ProducerEventN.begin(), ProducerEventN.end());
	LPWSTR ProducerEventName = &transformed_producer_event[0];


	hProducerEvent = CreateEvent(NULL, FALSE, FALSE, ProducerEventName);
	if (hProducerEvent == NULL)
		return GetLastError();



	fout.open(bin_file_name, std::ios::binary);

	prod p;
	p.q = 10 + (rand() % 30);
	for (int i = 0; i < p.q; ++i)
	{
		p.queue_[i] = (rand() % 1);
	}

	
	fout.write((char*)&p, sizeof(prod));

	fout.close();

	ifstream fin;
	fin.open(bin_file_name, std::ios::binary);
	fin.read((char*)&p, sizeof(prod));
	fin.close();

	string mutexn = "mutex";
	wstring transformed_mutex = wstring(mutexn.begin(), mutexn.end());
	LPWSTR mutexName = &transformed_mutex[0];


	hMutex = CreateMutex(NULL, FALSE, L"mutex");
	if (hMutex == NULL)
		return GetLastError();


	HANDLE* Events = new HANDLE[3];


	for (int i = 0; i < 3; ++i)
	{

		string EventN = "Process_started" + to_string(i);
		wstring transformed_event = wstring(EventN.begin(), EventN.end());
		LPWSTR eventName = &transformed_event[0];


		Events[i] = CreateEvent(NULL, FALSE, FALSE, eventName);
		if (Events[i] == NULL)
			return GetLastError();
	
		string creator = "Consumer.exe " + mutexn + " " + bin_file_name + " " + EventN + " " + ProducerEventN;
		wstring transformed_creator = wstring(creator.begin(), creator.end());
		LPWSTR szAppName = &transformed_creator[0];

		ZeroMemory(&si[i], sizeof(STARTUPINFO));
		si[i].cb = sizeof(STARTUPINFO);
		if (!CreateProcess(NULL, szAppName, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si[i], &pi[i]))
		{
			cout << "Error";
			return 0;
		}

		CloseHandle(pi[i].hProcess);
	}
	int first = 0, second = 0, third = 0;
	SetEvent(hProducerEvent);
	for (int j = 0; j < 2; ++j)
	{
		WaitForSingleObject(hProducerEvent, INFINITE);
		WaitForMultipleObjects(3, Events, TRUE, INFINITE);
		fout.open(bin_file_name, std::ios::binary);
		prod production;

		if (j == 0)
		{
			first = 10 + (rand() % 30);
			for (int i = 0; i < first; ++i)
			{
				production.queue_[i] = 0;
			}
			second = 10 + (rand() % 30);
			for (int i = first; i < first + second; ++i)
			{
				production.queue_[i] = 1;
			}
		}
		else if (j == 1)
		{
			first = 10 + (rand() % 30);
			for (int i = 0; i < first; ++i)
			{
				production.queue_[i] = 0;
			}
			second = 10 + (rand() % 30);
			for (int i = first; i < first + second; ++i)
			{
				production.queue_[i] = 1;
			}
			third = 10 + (rand() % 30);
			for (int i = first + second; i < first + second + third; ++i)
			{
				production.queue_[i] = 2;
			}
		}


		production.q = first + second + third;
		fout.write((char*)&production, sizeof(prod));

		fout.close();
		first = 0;
		second = 0;
		third = 0;
		SetEvent(hProducerEvent);
		ResetEvent(Events[0]);
		ResetEvent(Events[1]);
		ResetEvent(Events[2]);
	}


	CloseHandle(hMutex);
	CloseHandle(hProducerEvent);
	return 0;
}