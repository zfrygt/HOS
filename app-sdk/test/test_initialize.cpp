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
	connector->connect();

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