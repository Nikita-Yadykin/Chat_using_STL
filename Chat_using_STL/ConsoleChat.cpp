#include"ConsoleСhat.h"
#include<iostream>
#include <windows.h>
#include "sha1.h" // Подключаем заголовочный файл для хеширования SHA-1
#include <cstring>
using namespace std;

bool ConsoleChat::сhatStarted() const
{
	return _сhatStarted;
}

void ConsoleChat::start()
{
	_сhatStarted = true;
}

void ConsoleChat::startMenu() // Стартовое меню, отображается при запуске
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 10);
	cout << "\tWelcome to chat!\n\n"
		<< "Choose an action to continue: \n\n"
		<< "1-Register\n"
		<< "2-Enter chat\n"
		<< "3-Leave the chat\n";
	string str;
	char action = '0';

	cin >> str;

	if (!str.empty())
		action = '0';

	action = str[0];

	switch (action)
	{
	case '1':
		SetConsoleTextAttribute(hConsole, 10);
		signUp(); // Зарегистрироваться
		break;
	case '2':

		logIn(); // Войти
		break;
	case '3':
		_сhatStarted = false; // Выход из чата
		SetConsoleTextAttribute(hConsole, 15);
		break;
	default:
		SetConsoleTextAttribute(hConsole, 12);
		cout << "Please check that the characters you are entering are correct.\n";
		SetConsoleTextAttribute(hConsole, 11);
		break;
	}

};

shared_ptr<User> ConsoleChat::getUserByLogin(const string login) const
{
	auto it = _user.find(login);
	if (it != _user.end()) // Если введенный логин совпал с логином одного из пользователей
		return make_shared<User>(it->second); // Возвращаем указатель на этого пользователя
	return nullptr;

}

shared_ptr<User>  ConsoleChat::getOnlineUser() const
{
	return _onlineUser;
}

void ConsoleChat::logIn() // Вход в чат
{
	string login;
	string password;

	cout << "Please enter a login: ";
	cin >> login;
	cout << "Enter password: ";
	cin >> password;

	_onlineUser = getUserByLogin(login); // Указатель на онлайн пользователя получает указатель на пользователя с указанным логином

	if (_onlineUser == nullptr || !verifyPassword(password, _onlineUser->getPassword())) // Если нет онлайн пользователя или указанный пароль не соответствует хэшу пароля онлайн пользователя
	{
		_onlineUser = nullptr; // Сообщаем, что онлайн пользователь отсутствует
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, 12);
		cout << "Invalid login or password, please try again\n";
		SetConsoleTextAttribute(hConsole, 11);
	}
}

void ConsoleChat::signUp() // Регистрация
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 10);
	string login, password, name;

	cout << "Please enter your login: ";
	cin >> login;
	cout << "Choose security password: ";
	cin >> password;
	cout << "Enter your name: ";
	cin >> name;

	try
	{
		if (getUserByLogin(login) || login == "all") // Если пользователь с таким логином существует или логин = all
		{
			throw LoginException("error: "); // Уникальность логина
		};
	}
	catch (const LoginException& ex) // Ловим выброшенное исключение
	{
		SetConsoleTextAttribute(hConsole, 12);
		cerr << "\n\n (" << ex.what() << ") \n\n"; // Вызываем метод what()
		SetConsoleTextAttribute(hConsole, 15);
		return;
	}

	string hashedPassword = hashPassword(password); // Хешируем пароль

	User user = User(login, hashedPassword, name); // Создаем нового пользователя
	_user.insert(make_pair(login, user)); // Добавляем нового пользователя
	_onlineUser = make_shared<User>(user); // Новый пользователь становится онлайн пользователем
	cout << "\nCongratulations! You have successfully registered!\n";
}

string ConsoleChat::hashPassword(const string& password) // Функция для хеширования пароля
{
	uint* hashedPassword = sha1(password.c_str(), password.length());
	string hashStr;

	for (int i = 0; i < SHA1HASHLENGTHUINTS; ++i) {
		// Преобразуем каждое 32-битное слово хеша в строку шестнадцатеричного представления
		hashStr += to_string(hashedPassword[i]);
	}

	// Удаляем массив с хешем
	delete[] hashedPassword;

	return hashStr;
}

bool ConsoleChat::verifyPassword(const string& password, const string& hashedPassword) // Функция для проверки соответствия хэша пароля
{
	string hashedInput = hashPassword(password); // Хешируем введенный пароль
	return (hashedInput == hashedPassword); // Сравниваем хэши
}


void ConsoleChat::openChat() const // Просмотр чата и списка пользователей
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 11);
	string sender, recipient;

	cout << "-------users chatting online-----\n";
	cout << "login  \t\t  name  \n";
	cout << "\n---------------------------------\n";
	for (const auto& user : _user) // Перебираем пользователей
		cout << user.first << "\t---\t" << user.second.getName() << "\n"; // Выводим логин и имя пользователя
	cout << "\n--------messages-----------------\n\n";
	cout << " -----------chat-------  \n";

	for (const auto& pair : _message) // Перебираем сообщения
	{
		const Message& message = pair.second;
		if (_onlineUser->getLogin() == message.getSender() || _onlineUser->getLogin() == message.getRecipient() || message.getRecipient() == "all") // Если логин онлайн пользователя равен логину отправителя или логину получателя или равен "all"
		{
			sender = (_onlineUser->getLogin() == message.getSender()) ? _onlineUser->getLogin() : getUserByLogin(message.getSender())->getName(); // Устанавливаем отправителя

			if (message.getRecipient() == "all") // Если сообщение предназначено для всех
			{
				recipient = "all";
			}
			else // Если сообщение для конкретного пользователя
			{
				recipient = (_onlineUser->getLogin() == message.getRecipient()) ? _onlineUser->getLogin() : getUserByLogin(message.getRecipient())->getName(); // Устанавливаем получателя
			}
			cout << "message from " << sender << ": to " << recipient << "\n";
			cout << " - " << message.getLetter() << " - \n";
		}
	}
	cout << "---------------------------\n";

}

void ConsoleChat::sendMessage() // Написать сообщение
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	string recipient, text;
	cout << "to (login or all) - ";
	cin >> recipient; // Указать логин получателя или all
	cout << ">> ";
	cin.ignore();
	getline(cin, text);

	int messageId = _message.size() + 1; // Создаем уникальный идентификатор сообщения

	if (!(recipient == "all" || getUserByLogin(recipient))) {  // Если получатель не равен all или не найден логин пользователя

		SetConsoleTextAttribute(hConsole, 12);
		cout << "error no such user  -- " << recipient << "\n";
		SetConsoleTextAttribute(hConsole, 11);
		return;
	}
	if (recipient == "all") // Если сообщение для всех
		_message.insert(make_pair(messageId, Message(_onlineUser->getLogin(), "all", text))); // Создаем сообщение для всех
	//	_message.push_back(Message(_onlineUser->getLogin(), "all", text)); 
	else
		_message.insert(make_pair(messageId, Message(_onlineUser->getLogin(), getUserByLogin(recipient)->getLogin(), text))); // Создаем сообщение для конкретного пользователя
	//		_message.push_back(Message(_onlineUser->getLogin(), getUserByLogin(recipient)->getLogin(), text)); 
	cout << "message send -  " << recipient << ":\n ";
}

void ConsoleChat::chatMenu() // Меню чата
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 11);
	cout << "Welcome " << _onlineUser->getName() << "\n";
	while (_onlineUser) // Пока есть онлайн пользователь
	{
		cout << "Choose an action: \n"
			<< "1-Group chat \n"
			<< "2-Write a message \n"
			<< "3-Exit \n";
		string str;
		char action;

		cin >> str;

		if (!str.empty())
			action = '0';

		action = str[0];

		switch (action)
		{
		case '1':
			openChat(); // Открыть чат
			break;
		case '2':
			sendMessage(); // Написать сообщение
			break;
		case '3':
			_onlineUser = nullptr; // Выход
			break;
		default:
			cout << "Please try again\n";
			break;
		}
	}
}