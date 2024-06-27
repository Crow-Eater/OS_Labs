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
		cout << "Выберите метод\n1. Модифицировать запись\n2. Прочитать запись\n3. Завершить процесс\n";

		cin >> ans;

		if (ans == 1)
		{
			DWORD bytesWriten = 0;
			DWORD bytesRead = 0;

			int ID;
			cout << "Введите ID сотрудника: ";
			cin >> ID;

			int msg = ID * 10 + ans;
			bool writen = WriteFile(hPipe, &msg, sizeof(msg), &bytesWriten, NULL);
			
			if (writen)
			{
				cout << "Сообщение отправлено\n";
			}
			else
			{
				cout << "Сообщение не было отправлено\n";
			}

			employee* emp = new employee();
			bool read = ReadFile(hPipe, emp, sizeof(employee), &bytesRead, NULL);
			if (!read)
			{
				cout << "Сообщение не было прочитано\n";
			}

			if (emp->num == ID)
			{
				cout << "\nID: " << emp->num << "\n" << "Имя: " << emp->name << "\n" << "Отработанные часы: " << emp->hours << "\n";

				cout << "Введите новое имя: ";
				cin >> emp->name;

				cout << "Введите новое кол-во отработанных часов: ";
				cin >> emp->hours;
			}

			string str;

			while (true) 
			{
				cout << "Введите y, если хотите завершить работу с объектом\n";
				cin >> str;

				if (str == "y")
				{
					break;
				}
			}

			writen = WriteFile(hPipe, emp, sizeof(employee), &bytesWriten, NULL);
			if (writen)
			{
				cout << "Сообщение отправлено\n";
			}
			else
			{
				cout << "Сообщение не было отправлено\n";
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
			cout << "Введите ID сотрудника: ";
			cin >> ID;

			int msg = ID * 10 + ans;
			bool writen = WriteFile(hPipe, &msg, sizeof(msg), &bytesWritten, NULL);
			if (writen)
			{
				cout << "Сообщение отправлено\n";
			}
			else
			{
				cout << "Сообщение не было отправлено\n";
			}

			employee* emp = new employee();
			bool read = ReadFile(hPipe, emp, sizeof(employee), &bytesRead, NULL);
			if (!read)
			{
				cout << "Сообщение не было прочитано\n";
			}

			if (emp->num == ID)
			{
				cout << "\nID: " << emp->num << "\n" << "Имя: " << emp->name << "\n" << "Отработанные часы: " << emp->hours << "\n";
			}

			string str;
			while (true)
			{
				cout << "Введите y, если хотите завершить работу с объектом\n";
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
				cout << "Сообщение отправлено\n";
			}
			else
			{
				cout << "Сообщение не было отправлено\n";
			}

			break;
		}
		else
		{
			cout << "Некоректный ввод. Попробуйте ещё раз\n\n";
		}
	}

	DisconnectNamedPipe(hPipe);
	CloseHandle(hPipe);

	return 0;
}