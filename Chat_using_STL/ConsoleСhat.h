#pragma once
#include<map>
#include<memory>
#include"Message.h"

class LoginException :public exception // Класс исключения наследник от класса exception
{
public:
	LoginException(const std::string& msg) :_msg(msg) {}
	virtual const char* what() const noexcept override // Виртуальный метод what() выводит предупреждение, если логин не уникален
	{
		return "error: user login already exists ";
	}
private:
	std::string _msg;
};

class ConsoleChat
{
public:
	void start();
	bool сhatStarted() const;
	void startMenu();
	void signUp();
	void logIn();
	shared_ptr<User> getOnlineUser() const;
	void chatMenu();
	void sendMessage();
	string hashPassword(const string& password); // Функция для хеширования пароля
	bool verifyPassword(const string& password, const string& hashedPassword); // Функция для проверки соответствия хэша пароля
	
private:

	map<string, User> _user;
	shared_ptr<User> _onlineUser = nullptr;
	map<int, Message> _message;
	bool _сhatStarted = false;
	shared_ptr<User> getUserByLogin(const string login) const;
	void openChat() const;
	
};


