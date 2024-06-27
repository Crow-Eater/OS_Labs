#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include "queue.h"

using namespace std;
HANDLE hMutex;
HANDLE hConsumerEvent;
HANDLE hProducerEvent;
int main(int argc, char* argv[])
{
	cout << argv[1] << " " << argv[2] << " " << argv[3] <<  "\n";
	char c;


	string eventQ = argv[4];
	wstring transformed_event = wstring(eventQ.begin(), eventQ.end());
	LPWSTR eventName = &transformed_event[0];

	hProducerEvent = OpenEvent(EVENT_MODIFY_STATE, FALSE, eventName);
	if (hProducerEvent == NULL)
	{
		cout << "Open event failed." << endl;
		cout << "Input any char to exit." << endl;
		cin >> c;
		return GetLastError();
	}



	string ConsumerEventQ = argv[3];
	wstring transformed_consumer_event = wstring(ConsumerEventQ.begin(), ConsumerEventQ.end());
	LPWSTR ConsumerEventName = &transformed_consumer_event[0];

	hConsumerEvent = OpenEvent(EVENT_MODIFY_STATE, FALSE, ConsumerEventName);
	if (hConsumerEvent == NULL)
	{
		cout << "Open event failed." << endl;
		cout << "Input any char to exit." << endl;
		cin >> c;
		return GetLastError();
	}


	string mutexN = argv[1];
	wstring transformed_mutex = wstring(mutexN.begin(), mutexN.end());
	LPWSTR mutexName = &transformed_mutex[0];

	hMutex = OpenMutex(SYNCHRONIZE, FALSE, L"mutex");
	if (hMutex == NULL)
	{
		cout << "Open mutex failed." << endl;
		cout << "Input any char to exit." << endl;
		cin >> c;
		return GetLastError();
	}

	string bin_file_name = argv[2];
	ifstream fin;
	prod p;
	int count = 0;
	while (true)
	{
		WaitForSingleObject(hConsumerEvent, INFINITE);
		while (true)
		{
			WaitForSingleObject(hMutex, INFINITE);
			fin.open(bin_file_name, std::ios::binary);
			fin.read((char*)&p, sizeof(prod));

			cout << p.q << "\n";

			int pr = p.queue_[p.q - 1];

			switch (pr)
			{
			case 0:
			{
				cout << "Table\n";
				break;
			}
			case 1:
			{
				cout << "Chair\n";
				break;
			}
			case 2:
			{
				cout << "Bookshelf\n";
				break;
			}
			}
			fin.close();
			ofstream file(bin_file_name, std::ios::binary);
			p.q--;
			if (p.q <= 2)
			{
				file.write((char*)&p, sizeof(prod));

				file.close();

				ReleaseMutex(hMutex);
				SetEvent(hConsumerEvent);

				break;
			}
			file.write((char*)&p, sizeof(prod));

			file.close();
			ReleaseMutex(hMutex);
		}
		cout << "------------------------------\n";
		WaitForSingleObject(hProducerEvent, INFINITE);
		count++;
		if (count == 3)
		{
			break;
		}
	}

	CloseHandle(hConsumerEvent);
	cout << "Now input any char to exit from the process: ";
	cin >> c;
	getchar();
	return 0;
}