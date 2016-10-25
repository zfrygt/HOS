#include <zmq.h>
#include <connector.h>
#include <assert.h>
#include <hos_protocol.pb.h>
#include <iostream>

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

	connector->send("murat", 5);

	char rec_buf[80] = { 0 };

	std::string clientName;
	clientName.reserve(10);
	
	// read identity
	auto ret_bytes = zmq_recv(server_socket, rec_buf, sizeof rec_buf, 0);
	strcpy(&clientName[0], rec_buf); // clientName lenght DOES NOT CHANGE!!! DO NOT TRY TO GET ClientName lenght!!
	assert(ret_bytes == module_name.length());
	assert(strncmp(clientName.c_str(), module_name.c_str(), ret_bytes) == 0);

	//read empty delimiter
	memset(rec_buf, 0, sizeof rec_buf);
	auto ret_bytes2 = zmq_recv(server_socket, rec_buf, sizeof rec_buf, 0);
	assert(ret_bytes2 == 0);
	assert(strncmp(rec_buf, "", ret_bytes2) == 0);

	//read data
	std::string data;
	data.reserve(10);
	memset(rec_buf, 0, sizeof rec_buf);
	auto ret_bytes3 = zmq_recv(server_socket, rec_buf, sizeof rec_buf, 0);
	strcpy(&data[0], rec_buf);
	assert(ret_bytes3 == std::string("murat").length());
	assert(strncmp(data.c_str(), "murat", ret_bytes3) == 0);
	
	int r1 = zmq_send(server_socket, clientName.c_str(), ret_bytes, ZMQ_SNDMORE);
	assert(r1 == 4);

	int r2 = zmq_send(server_socket, nullptr, 0, ZMQ_SNDMORE);
	assert(r2 == 0);

	std::string output("murat");

	// Send command id
	int r3 = zmq_send(server_socket, output.c_str(), output.length(), 0);
	assert(r3 == output.length());

	connector->heartbeat(25);
	delete connector;

	zmq_close(server_socket);
	zmq_ctx_destroy(context);

	return 0;
}