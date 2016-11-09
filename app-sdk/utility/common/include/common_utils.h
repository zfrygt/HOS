#ifndef COMMON_UTILS_H
#define COMMON_UTILS_H

#include <zmq.h>
#include <hos_protocol.pb.h>
#include <serializer.h>
#include <memory>
#include <chrono>

#define HEARTHBEAT_INTERVAL_IN_SECONDS 5 
#define TIMEOUT_INTERVAL_IN_SECONDS (3 * HEARTHBEAT_INTERVAL_IN_SECONDS)
#define TIMEOUT_CHECK_INTERVAL_IN_SECONDS 2
#include <thread>
#include <envelope.h>

inline int64_t current_time(){
	auto temp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	return static_cast<int64_t>(temp) / 1000;
}

inline void hos_sleep(uint64_t ms)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

using ProtobufMessageEnvelope = Envelope<::google::protobuf::Message>;

template<typename T>
inline void send_message(void* socket, const T* message)
{
	assert(socket != nullptr);
	assert(message != nullptr);

	auto so = Serializer::serialize(message);

	auto buf = so->get_buf();
	auto size = so->get_size();

	assert(buf != nullptr);
	assert(size > 0);

	// Send empty frame
	zmq_send(socket, nullptr, 0, ZMQ_SNDMORE);

	// Send data frame
	zmq_send(socket, buf, size, 0);
}

template <typename T>
inline std::unique_ptr<T> recv_message(void* socket)
{
	assert(socket != nullptr);

	char buffer[80] = { 0 };

	// read empty frame
	auto len_ef = zmq_recv(socket, buffer, sizeof buffer, 0);
	assert(len_ef == 0);

	// read data
	auto data_size = zmq_recv(socket, buffer, sizeof buffer, 0);
	assert(data_size != -1);

	auto so = std::make_unique<SerializedObject>(data_size);

	so->copyFrom(buffer);

	return Serializer::deserialize<T>(so.get());
}

inline void send_server_message(void* socket, const ProtobufMessageEnvelope* server_message)
{
	assert(server_message != nullptr);

	auto c = server_message->to.c_str();
	auto s = server_message->to.length();

	assert(c != nullptr);
	assert(s > 0);

	// send client id
	zmq_send(socket, c, s, ZMQ_SNDMORE);
	send_message(socket, server_message->payload.get());
}

inline void send_client_message(void* socket, const ProtobufMessageEnvelope* client_message)
{
	send_message(socket, client_message->payload.get());
}

inline ProtobufMessageEnvelope recv_client_message(void* socket)
{
	// get client identifier
	char buffer[80] = { 0 };
	auto len_id = zmq_recv(socket, buffer, sizeof buffer, 0);
	assert(len_id > 0);

	return ProtobufMessageEnvelope(recv_message<ClientMessage>(socket), "", std::string(buffer));
}

inline ProtobufMessageEnvelope recv_server_message(void* socket)
{
	return ProtobufMessageEnvelope(recv_message<ServerMessage>(socket));
}

#endif