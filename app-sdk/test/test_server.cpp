#include <utils.h>
#include <connector.h>
#include <thread>
#include <future>
#include <iostream>
#include <server.h>

int main()
{
	Server server("tcp://*:5555");
	server.start();
	const std::string module_name("test");

	auto connector = new Connector("tcp://localhost:5555", module_name.c_str());
	connector->connect();

	//auto connector_loop = []()
	//{

	//	while (true)
	//	{
	//		connector->heartbeat(50);
	//	}
	//	delete connector;
	//};

	//auto conn_feature = std::async(std::launch::async, connector_loop);

	//conn_feature.get();

	std::cin.get();

	return 0;
}

