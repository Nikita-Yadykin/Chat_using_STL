#include"Message.h"

Message::Message(const string& sender, const string& recipient, const string& letter) : _sender(sender), _recipient(recipient), _letter(letter)
{
}

string Message::getSender() const
{
	return _sender;
}
string  Message::getRecipient() const
{
	return _recipient;
}
string  Message::getLetter() const
{
	return _letter;
}