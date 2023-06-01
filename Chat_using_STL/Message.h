#pragma once
#include"User.h"

class Message
{
public:

	Message(const string& sender, const string& recipient, const string& letter);
	~Message() = default;

	string getSender() const; // Получить отправителя
	string getRecipient() const; // Получить получателя
	string getLetter() const; // Получить текст сообщения

private:

	const string _sender;
	const string _recipient;
	const string _letter;

};
