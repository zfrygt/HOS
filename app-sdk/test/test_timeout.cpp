#include <utils.h>
#include <connector.h>
#include <thread>
#include <future>
#include <iostream>

int main()
{
	auto context = zmq_ctx_new();
	assert(context != nullptr);

	auto server_socket = zmq_socket(context, ZMQ_ROUTER);
	assert(server_socket != nullptr);

	auto val = 1;
	auto xx = zmq_setsockopt(server_socket, ZMQ_ROUTER_MANDATORY, &val, sizeof val);
	assert(xx == 0);

#ifdef USE_CURVE
	int opt = 1;
	std::string secretKey("Sd[BRNU[GQ6YL<P5-O!b]{pD@^yxNQ).Iln9%eU1");

	auto curve_server = zmq_setsockopt(server_socket, ZMQ_CURVE_SERVER, &opt, sizeof opt);
	assert(curve_server == 0);

	auto secret_key = zmq_setsockopt(server_socket, ZMQ_CURVE_SECRETKEY, secretKey.c_str(), secretKey.length());
	assert(secret_key == 0);
#endif

	auto bound = zmq_bind(server_socket, "tcp://*:5555");
	assert(bound == 0);

	const std::string module_name("test");

    auto connector = new Connector("tcp://localhost:5555", module_name.c_str());

    auto connector_loop = [connector]()
	{
		connector->connect();

        while (connector->connected())
		{
            connector->heartbeat(50);
		}
	};

	auto conn_feature = std::async(std::launch::async, connector_loop);

	std::string client_name;
	auto client_message = recv_client_message(server_socket, client_name);
	assert(client_message->type() == Init);

	conn_feature.get();

    delete connector;

	zmq_close(server_socket);
	zmq_ctx_destroy(context);

	return 0;
}

