#include"User.h"

User::User(const string& login, string& password, string& name) : _login(login), _password(password), _name(name)
{
}

string User::getLogin() const
{
	return _login;
}
string  User::getPassword() const
{
	return _password;
}
string  User::getName() const
{
	return _name;
}