#include <conio.h>
#include <windows.h>
#include <iostream>

using std::cin;
using std::cout;
using std::string;

struct employee
{
	int num;
	char name[10];
	double hours;
};

int main(int argc, char* argv)
{
	setlocale(LC_ALL, "ru");

	HANDLE hEvent = OpenEvent(EVENT_MODIFY_STATE, FALSE, L"Process Started");
	SetEvent(hEvent);

	HANDLE hPipe = CreateFile(L"\\\\.\\pipe\\qwe", GENERIC_WRITE | GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);

	int ans = 0;
	while (true)
	{
		cout << "�������� �����\n1. �������������� ������\n2. ��������� ������\n3. ��������� �������\n";

		cin >> ans;

		if (ans == 1)
		{
			DWORD bytesWriten = 0;
			DWORD bytesRead = 0;

			int ID;
			cout << "������� ID ����������: ";
			cin >> ID;

			int msg = ID * 10 + ans;
			bool writen = WriteFile(hPipe, &msg, sizeof(msg), &bytesWriten, NULL);
			
			if (writen)
			{
				cout << "��������� ����������\n";
			}
			else
			{
				cout << "��������� �� ���� ����������\n";
			}

			employee* emp = new employee();
			bool read = ReadFile(hPipe, emp, sizeof(employee), &bytesRead, NULL);
			if (!read)
			{
				cout << "��������� �� ���� ���������\n";
			}

			if (emp->num == ID)
			{
				cout << "\nID: " << emp->num << "\n" << "���: " << emp->name << "\n" << "������������ ����: " << emp->hours << "\n";

				cout << "������� ����� ���: ";
				cin >> emp->name;

				cout << "������� ����� ���-�� ������������ �����: ";
				cin >> emp->hours;
			}

			string str;

			while (true) 
			{
				cout << "������� y, ���� ������ ��������� ������ � ��������\n";
				cin >> str;

				if (str == "y")
				{
					break;
				}
			}

			writen = WriteFile(hPipe, emp, sizeof(employee), &bytesWriten, NULL);
			if (writen)
			{
				cout << "��������� ����������\n";
			}
			else
			{
				cout << "��������� �� ���� ����������\n";
			}
			

			msg = 1;
			WriteFile(hPipe, &msg, sizeof(msg), &bytesWriten, NULL);
			delete emp;
		}
		else if (ans == 2)
		{
			DWORD bytesWritten = 0;
			DWORD bytesRead = 0;

			int ID;
			cout << "������� ID ����������: ";
			cin >> ID;

			int msg = ID * 10 + ans;
			bool writen = WriteFile(hPipe, &msg, sizeof(msg), &bytesWritten, NULL);
			if (writen)
			{
				cout << "��������� ����������\n";
			}
			else
			{
				cout << "��������� �� ���� ����������\n";
			}

			employee* emp = new employee();
			bool read = ReadFile(hPipe, emp, sizeof(employee), &bytesRead, NULL);
			if (!read)
			{
				cout << "��������� �� ���� ���������\n";
			}

			if (emp->num == ID)
			{
				cout << "\nID: " << emp->num << "\n" << "���: " << emp->name << "\n" << "������������ ����: " << emp->hours << "\n";
			}

			string str;
			while (true)
			{
				cout << "������� y, ���� ������ ��������� ������ � ��������\n";
				cin >> str;

				if (str == "y")
				{
					break;
				}
			}

			msg = 1;
			WriteFile(hPipe, &msg, sizeof(msg), &bytesWritten, NULL);
		}
		else if (ans == 3)
		{
			DWORD bytesWritten;
			DWORD bytesRead;

			int msg = ans;
			bool writen = WriteFile(hPipe, &msg, sizeof(msg), &bytesWritten, NULL);
			if (writen)
			{
				cout << "��������� ����������\n";
			}
			else
			{
				cout << "��������� �� ���� ����������\n";
			}

			break;
		}
		else
		{
			cout << "����������� ����. ���������� ��� ���\n\n";
		}
	}

	DisconnectNamedPipe(hPipe);
	CloseHandle(hPipe);

	return 0;
}