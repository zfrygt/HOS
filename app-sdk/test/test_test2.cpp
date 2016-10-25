#include <zmq.h>
#include <connector.h>
#include <assert.h>
#include <hos_protocol.pb.h>
#include <iostream>
#include <serializer.h>
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

	char buffer[80] = { 0 };
	zmq_recv(server_socket, buffer, sizeof buffer, 0);
	std::string client_name(buffer);
	memset(buffer, 0, sizeof(buffer));
	zmq_recv(server_socket, buffer, sizeof buffer, 0);
	zmq_recv(server_socket, buffer, sizeof buffer, 0);

	ServerMessage req;
	req.set_type(ServerMessage_Type_Ping);
	auto aa = Serializer::serialize(&req);

	auto buf = aa->get_buf();
	auto size = aa->get_size();

	auto c = client_name.c_str();
	auto s = client_name.length();

	zmq_send(server_socket, c, s, ZMQ_SNDMORE);
	zmq_send(server_socket, nullptr, 0, ZMQ_SNDMORE);
	zmq_send(server_socket, buf, size, 0);

	connector->heartbeat(25);
	delete connector;

	zmq_close(server_socket);
	zmq_ctx_destroy(context);

	return 0;
}