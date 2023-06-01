#pragma once
#include<map>
#include<memory>
#include"Message.h"

class LoginException :public exception // ����� ���������� ��������� �� ������ exception
{
public:
	LoginException(const std::string& msg) :_msg(msg) {}
	virtual const char* what() const noexcept override // ����������� ����� what() ������� ��������������, ���� ����� �� ��������
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
	bool �hatStarted() const;
	void startMenu();
	void signUp();
	void logIn();
	shared_ptr<User> getOnlineUser() const;
	void chatMenu();
	void sendMessage();
	string hashPassword(const string& password); // ������� ��� ����������� ������
	bool verifyPassword(const string& password, const string& hashedPassword); // ������� ��� �������� ������������ ���� ������
	
private:

	map<string, User> _user;
	shared_ptr<User> _onlineUser = nullptr;
	map<int, Message> _message;
	bool _�hatStarted = false;
	shared_ptr<User> getUserByLogin(const string login) const;
	void openChat() const;
	
};


