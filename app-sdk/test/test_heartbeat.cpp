#include <zmq.h>
#include <connector.h>
#include <assert.h>
#include <hos_protocol.pb.h>
#include <serializer.h>
#include <thread>
#include <future>
#include <iostream>

void sendHeartbeat(void*, const std::string&);
std::unique_ptr<ClientMessage> recv_client_message(void*, std::string&);

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

	auto connector_loop = [module_name]()
	{
		auto connector = new Connector("tcp://localhost:5555", module_name.c_str());

		auto counter = 4;
		while (counter--)
		{
			connector->heartbeat(-1);
		}

		delete connector;
	};

	auto conn_feature = std::async(std::launch::async, connector_loop);

	std::string client_name;
	auto client_message = recv_client_message(server_socket, client_name);
	assert(client_message->type() == Init);

	auto counter = 4;

	while (counter--)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(6000));

		sendHeartbeat(server_socket, client_name);

		std::string client_name_ping;
		auto client = recv_client_message(server_socket, client_name_ping);
		assert(client->type() == Pong);

		std::cout << "Round trip: " << counter << '\n';
	}

	conn_feature.get();

	zmq_close(server_socket);
	zmq_ctx_destroy(context);

	return 0;
}

inline void sendHeartbeat(void* socket, const std::string& client_name)
{
	ServerMessage req;
	req.set_type(Ping);
	auto so = Serializer::serialize(&req);

	auto buf = so->get_buf();
	auto size = so->get_size();

	auto c = client_name.c_str();
	auto s = client_name.length();

	assert(c != nullptr);
	assert(s > 0);

	zmq_send(socket, c, s, ZMQ_SNDMORE);
	zmq_send(socket, nullptr, 0, ZMQ_SNDMORE);
	zmq_send(socket, buf, size, 0);
}

std::unique_ptr<ClientMessage> recv_client_message(void* socket, std::string& client)
{
	// get client identifier
	char buffer[80] = { 0 };
	auto len_id = zmq_recv(socket, buffer, sizeof buffer, 0);
	assert(len_id > 0);

	client = std::string(buffer);

	// read empty frame
	auto len_ef = zmq_recv(socket, buffer, sizeof buffer, 0);
	assert(len_ef == 0);

	// read data
	auto init_mes_size = zmq_recv(socket, buffer, sizeof buffer, 0);
	assert(init_mes_size != -1);

	// make sure the client sends init command when it connects to server.
	auto so = std::make_unique<SerializedObject>(init_mes_size);
	so->copyFrom(buffer);

	auto client_message = Serializer::deserialize<ClientMessage>(std::move(so));

	return client_message;
}