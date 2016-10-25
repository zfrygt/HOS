#include <zmq.h>
#include <connector.h>
#include <assert.h>
#include <hos_protocol.pb.h>
#include <iostream>
#include <serializer.h>

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
	memset(rec_buf, 0, sizeof rec_buf);


	auto ret_bytes3 = zmq_recv(server_socket, rec_buf, sizeof rec_buf, 0);
	auto temp_buf = malloc(ret_bytes3);
	memcpy(temp_buf, rec_buf, ret_bytes3);

	auto data = move(Serializer::deserialize<ClientMessage>(std::make_unique<SerializedObject>(temp_buf, ret_bytes3)));
	assert(data->type() == ClientMessage_Type_Init);
	
	delete connector;

	zmq_close(server_socket);
	zmq_ctx_destroy(context);

	return 0;
}