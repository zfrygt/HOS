#ifndef UTILS_H
#define UTILS_H

#include <zmq.h>
#include <iostream>
#include <hos_protocol.pb.h>
#include <serializer.h>

static inline std::string array_to_string(const char* arr)
{
	return std::string(arr);
}

inline void send_server_message(void* socket, const ServerMessage* server_message, const std::string& client_name)
{
	auto c = client_name.c_str();
	auto s = client_name.length();

	assert(c != nullptr);
	assert(s > 0);

	auto so = Serializer::serialize(server_message);

	auto buf = so->get_buf();
	auto size = so->get_size();

	zmq_send(socket, c, s, ZMQ_SNDMORE);
	zmq_send(socket, nullptr, 0, ZMQ_SNDMORE);
	zmq_send(socket, buf, size, 0);
}

inline std::unique_ptr<ClientMessage> recv_client_message(void* socket, std::string& client)
{
	// get client identifier
	char buffer[80] = { 0 };
	auto len_id = zmq_recv(socket, buffer, sizeof buffer, 0);
	assert(len_id > 0);

	client = std::move(std::string(buffer));

	// read empty frame
	auto len_ef = zmq_recv(socket, buffer, sizeof buffer, 0);
	assert(len_ef == 0);

	// read data
	auto data_size = zmq_recv(socket, buffer, sizeof buffer, 0);
	assert(data_size != -1);

	// make sure the client sends init command when it connects to server.
	auto so = std::make_unique<SerializedObject>(data_size);
	so->copyFrom(buffer);

	return Serializer::deserialize<ClientMessage>(so.get());
}

#endif