#include <zmq.h>
#include <connector.h>
#include <assert.h>
#include <utils.h>

int main()
{
	auto context = zmq_ctx_new();
	assert(context != nullptr);

	auto server_socket = zmq_socket(context, ZMQ_ROUTER);
	assert(context != nullptr);

	auto val = 1;
	auto xx = zmq_setsockopt(server_socket, ZMQ_ROUTER_MANDATORY, &val, sizeof val);
	assert(xx == 0);

	auto bound = zmq_bind(server_socket, "tcp://*:5555");
	assert(bound == 0);

	const std::string module_name("test");

	auto connector = new Connector("tcp://localhost:5555", module_name.c_str());

	std::string clientName;

	// receive client init request
	auto client_message = recv_client_message(server_socket, clientName);
	assert(strncmp(clientName.c_str(), module_name.c_str(), module_name.length()) == 0);
	assert(client_message->type() == Init);
	
	delete connector;

	zmq_close(server_socket);
	zmq_ctx_destroy(context);

	return 0;
}