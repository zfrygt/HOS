#include <server_connector.h>
#include <iostream>
#include <spdlog/spdlog.h>

int main()
{
	ServerConnector connector("tcp://*:5555", spdlog::stdout_color_mt("supervisor"));
	connector.start();

	std::cin.get();

	return 0;
}