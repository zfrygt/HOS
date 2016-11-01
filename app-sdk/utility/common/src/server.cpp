#include <server.h>
#include <hos_protocol.pb.h>
#include <zmq.h>
#include <utils.h>
#include <iostream>
#include <job_ping.h>
#include <future>
#include <async_job_queue.h>
#include <client.h>

tbb::tbb_thread* server_thread = nullptr;

Server::Server(const char* uri):
m_started(false),
m_job_queue(new AsyncJobQueue<IJob>)
{
	m_context = zmq_ctx_new();
	assert(m_context != nullptr);

	m_socket = zmq_socket(m_context, ZMQ_ROUTER);
	assert(m_socket != nullptr);

	auto val = 1;
	auto xx = zmq_setsockopt(m_socket, ZMQ_ROUTER_MANDATORY, &val, sizeof val);
	assert(xx == 0);

	auto linger = 0;
	auto r = zmq_setsockopt(m_socket, ZMQ_LINGER, &linger, sizeof(linger)); // close cagirildiktan sonra beklemeden socket'i kapat.
	assert(r == 0);

#ifdef USE_CURVE
	auto opt = 1;
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
	stop();

	if (m_job_queue)
	{
		delete m_job_queue;
		m_job_queue = nullptr;
	}

	zmq_close(m_socket);
	zmq_ctx_destroy(m_context);
}

void Server::heartbeat(long timeout)
{
	zmq_pollitem_t items[] = {
		{ m_socket, 0, ZMQ_POLLIN, 0 }
	};
	zmq_poll(items, 1, timeout);
	if (items[0].revents & ZMQ_POLLIN){
		on_receive();
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
			m_job_queue->add_job(std::make_shared<JobPing>(this, c->get_client_name()));
		}
		++it;
	}
}

void Server::start()
{
	if (!m_started && server_thread == nullptr)
	{
		m_started = true;

		server_thread = new tbb::tbb_thread([](Server* srv)
		{
			assert(srv != nullptr);
			while (srv->m_started)
				srv->heartbeat(25);
		}, this);
	}
}

void Server::stop()
{
	if (m_started && server_thread != nullptr)
	{
		m_started = false;
		server_thread->join();
		delete server_thread;
		server_thread = nullptr;
		delete m_job_queue;
		m_job_queue = nullptr;
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

void Server::on_receive()
{
	Client *client = nullptr;
	auto msg = receive(&client);
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

void Server::send(Client* client, const ServerMessage* server_message)
{
	assert(client != nullptr);
	assert(server_message != nullptr);
	send_server_message(m_socket, server_message, client->get_client_name());
	client->lastSendMessageTime = current_time();
}

void Server::send(const std::string& client_name, const ServerMessage* server_message)
{
	assert(!client_name.empty());
	auto found_client = m_client_map.find(client_name);
	assert(found_client != m_client_map.end());
	send(found_client->second, server_message);
}