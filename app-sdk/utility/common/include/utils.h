#ifndef UTILS_H
#define UTILS_H

#include <zmq.h>
#include <hos_protocol.pb.h>
#include <serializer.h>
#include <memory>
#include <chrono>

#define HEARTHBEAT_INTERVAL_IN_SECONDS 5 
#define TIMEOUT_INTERVAL_IN_SECONDS (3 * HEARTHBEAT_INTERVAL_IN_SECONDS)
#define TIMEOUT_CHECK_INTERVAL_IN_SECONDS 2
#include <thread>

class no_copy_move
{
public:
	no_copy_move() { }

protected:

	no_copy_move(const no_copy_move& other) = delete;
	no_copy_move(no_copy_move&& other) = delete;
	no_copy_move& operator=(const no_copy_move& other) = delete;
	no_copy_move& operator=(no_copy_move&& other) = delete;
};

inline int64_t current_time(){
	auto temp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	return static_cast<int64_t>(temp) / 1000;
}

inline void hos_sleep(uint64_t ms)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

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

#if defined(_WIN32) && defined(_MSC_VER)
	auto so = std::make_unique<SerializedObject>(data_size);
#else
	std::unique_ptr<SerializedObject> so(new SerializedObject(data_size));
#endif

	so->copyFrom(buffer);

	return Serializer::deserialize<T>(so.get());
}

inline void send_server_message(void* socket, const ServerMessage* server_message, const std::string& client_name)
{
	auto c = client_name.c_str();
	auto s = client_name.length();

	assert(c != nullptr);
	assert(s > 0);

	// send client id
	zmq_send(socket, c, s, ZMQ_SNDMORE);
	send_message(socket, server_message);
}

inline void send_client_message(void* socket, const ClientMessage* client_message)
{
	send_message(socket, client_message);
}

inline std::unique_ptr<ClientMessage> recv_client_message(void* socket, std::string& client)
{
	// get client identifier
	char buffer[80] = { 0 };
	auto len_id = zmq_recv(socket, buffer, sizeof buffer, 0);
	assert(len_id > 0);

	client = std::move(std::string(buffer));

	return recv_message<ClientMessage>(socket);
}

inline std::unique_ptr<ServerMessage> recv_server_message(void* socket)
{
	return recv_message<ServerMessage>(socket);
}

#endif