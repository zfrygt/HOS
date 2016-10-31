#include <utils.h>
#include <connector.h>
#include <thread>
#include <future>
#include <iostream>
#include <server.h>

int main()
{

	const std::string module_name("test");

	auto connector = new Connector("tcp://localhost:5555", module_name.c_str());
	connector->connect();

	auto connector_loop = [connector]()
	{
		while (!connector->timeout())
		{
			connector->heartbeat(50);
		}
	};

	auto conn_feature = std::async(std::launch::async, connector_loop);

	Server server("tcp://*:5555");
	server.start();

	auto counter = 100;

	do
	{
		server.heartbeat(50);
		
	} while (counter--);

	conn_feature.get();

	delete connector;

	return 0;
}

