#include <server.h>
#include <hos_protocol.pb.h>
#include <zmq.h>
#include <utils.h>
#include <iostream>
#include <tbb/concurrent_queue.h>
#include <job_ping.h>
#include <future>

Server::Server(const char* uri):
m_started(false),
m_job_queue(new job_queue),
m_job_thread(nullptr)
{
	m_context = zmq_ctx_new();
	assert(m_context != nullptr);

	m_socket = zmq_socket(m_context, ZMQ_ROUTER);
	assert(m_socket != nullptr);

	auto val = 1;
	auto xx = zmq_setsockopt(m_socket, ZMQ_ROUTER_MANDATORY, &val, sizeof val);
	assert(xx == 0);

#ifdef USE_CURVE
	int opt = 1;
	std::string secretKey("Sd[BRNU[GQ6YL<P5-O!b]{pD@^yxNQ).Iln9%eU1");

	auto curve_server = zmq_setsockopt(m_socket, ZMQ_CURVE_SERVER, &opt, sizeof opt);
	assert(curve_server == 0);

	auto secret_key = zmq_setsockopt(m_socket, ZMQ_CURVE_SECRETKEY, secretKey.c_str(), secretKey.length());
	assert(secret_key == 0);
#endif

	auto bound = zmq_bind(m_socket, uri);
	assert(bound == 0);
}

Server::~Server()
{
	zmq_close(m_socket);
	zmq_ctx_destroy(m_context);
}

void Server::heartbeat(long timeout)
{
	std::unique_ptr<ClientMessage> msg;

	zmq_pollitem_t items[] = {
		{ m_socket, 0, ZMQ_POLLIN, 0 }
	};
	zmq_poll(items, 1, timeout);
	if (items[0].revents & ZMQ_POLLIN){
		Client *client = nullptr;
		msg = receive(&client);
		assert(client != nullptr);

		switch (msg->type())
		{
		case Pong:

			break;
		case Init:
		{
			ServerMessage server_message;
			server_message.set_type(Init);
			send(client, &server_message);
		}
			break;
		default: break;
		}
	}

	auto currentTime = current_time();

	for (auto it = m_client_map.begin(); it != m_client_map.end();)
	{
		auto c = it->second;
		auto secondsSinceLastMessageSend = currentTime - c->lastSendMessageTime;
		auto secondsSinceLastMessageReceived = currentTime - c->lastReceivedMessageTime;
		if (c->lastReceivedMessageTime >= 0 && secondsSinceLastMessageReceived > TIMEOUT_INTERVAL_IN_SECONDS){
			// Timeout this client!
			std::cout << "dropping " << c->get_client_name() << " from the list\n";
			delete c;
			it = m_client_map.erase(it);
			continue;
		}
		if (c->lastSendMessageTime >= 0 && secondsSinceLastMessageSend > HEARTHBEAT_INTERVAL_IN_SECONDS){
			m_job_queue->push(std::make_shared<JobPing>(this, c));
		}
		++it;
	}
}

void Server::start()
{
	if (!m_started && m_job_thread == nullptr)
	{
		m_started = true;
		auto job_queue = m_job_queue;

		auto loop = [job_queue]()
		{
			assert(job_queue != nullptr);

			while (true)
			{
				std::shared_ptr<IJob> job;
				job_queue->pop(job);
				if (!job) {
					std::cout << "breaking!\n";
					break;
				}
				job->execute();
			}
		};

		m_job_thread = new std::future<void>(std::move(std::async(std::launch::async, loop)));
	}
}

std::unique_ptr<ClientMessage> Server::receive(Client** sender_client)
{
	assert(*sender_client == nullptr);

	std::string client_name;
	auto msg = recv_client_message(m_socket, client_name);
	auto found_client = m_client_map.find(client_name);

	// new client connected
	if (found_client == m_client_map.end())
		m_client_map[client_name] = new Client(client_name);

	*sender_client = m_client_map[client_name];
	(*sender_client)->lastReceivedMessageTime = current_time();

	return msg;
}

void Server::send(Client* client, const ServerMessage* server_message)
{
	assert(client != nullptr);
	send_server_message(m_socket, server_message, client->get_client_name());
	client->lastSendMessageTime = current_time();
}