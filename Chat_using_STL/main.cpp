#include"Console�hat.h"


auto main() -> int
{
	ConsoleChat ConsoleChat;

	ConsoleChat.start(); // ������ ����

	while (ConsoleChat.�hatStarted()) // ���� ��� �������
	{
		ConsoleChat.startMenu(); // ���������� ��������� ����

		while (ConsoleChat.getOnlineUser()) // ���� � ������� ���� ������������
		{
			ConsoleChat.chatMenu(); // ���������� ���� ����
		}
	}

	return 0;

}