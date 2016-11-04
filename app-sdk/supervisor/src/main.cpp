#include <server_connector.h>
#include <iostream>

int main()
{
	ServerConnector connector("tcp://*:5555");
	connector.start();

	std::cin.get();

	return 0;
}