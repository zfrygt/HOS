#include <server_connector.h>
#include <zmq.h>
#include <hos_protocol.pb.h>
#include <spdlog/spdlog.h>
#include <common_utils.h>
#include <future>
#include <async_job_queue.h>
#include <client.h>
#include <job_ping.h>

tbb::tbb_thread* server_thread = nullptr;

ServerConnector::ServerConnector(IReceivePolicy* receive_strategy, const char* uri, std::shared_ptr<spdlog::logger> logger) :
ConnectorBase(receive_strategy, std::forward<std::shared_ptr<spdlog::logger>>(logger), uri),
m_started(false)
{

}

ServerConnector::~ServerConnector()
{
	ServerConnector::destroy();
}

void ServerConnector::poll(long timeout)
{
	ConnectorBase::poll(timeout);

	auto currentTime = current_time();

	for (auto it = m_client_map.begin(); it != m_client_map.end();)
	{
		auto c = it->second;
		auto secondsSinceLastMessageSend = currentTime - c->lastSendMessageTime;
		auto secondsSinceLastMessageReceived = currentTime - c->lastReceivedMessageTime;
		if (c->lastReceivedMessageTime >= 0 && secondsSinceLastMessageReceived > TIMEOUT_INTERVAL_IN_SECONDS){
			// Timeout this client!
			m_logger->warn("[{}] disconnected", c->get_client_name());
			delete c;
			it = m_client_map.erase(it);
			continue;
		}
		if (c->lastSendMessageTime >= 0 && secondsSinceLastMessageSend > HEARTHBEAT_INTERVAL_IN_SECONDS){
			JobPing job_ping(this, c->get_client_name());
			job_ping.execute();
		}
		++it;
	}
}

void ServerConnector::start()
{
	init();

	if (!m_started && server_thread == nullptr)
	{
		m_started = true;

		server_thread = new tbb::tbb_thread([](ServerConnector* srv)
		{
			assert(srv != nullptr);
			while (srv->m_started)
				srv->poll(25);
		}, this);
	}
}

void ServerConnector::destroy()
{
	if (m_started && server_thread != nullptr)
	{
		m_started = false;
		server_thread->join();
		delete server_thread;
		server_thread = nullptr;
	}

	if (m_socket != nullptr)
	{
		zmq_close(m_socket);
		m_socket = nullptr;
	}

	if (m_context != nullptr)
	{
		zmq_ctx_destroy(m_context);
		m_context = nullptr;
	}
}

void ServerConnector::init()
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

	auto bound = zmq_bind(m_socket, m_uri.c_str());
	assert(bound == 0);
}

void ServerConnector::send(Envelope<google::protobuf::Message>* envelope)
{
	assert(envelope != nullptr);
	assert(!envelope->to.empty());
	auto found_client = m_client_map.find(envelope->to);
	assert(found_client != m_client_map.end());
	send_server_message(m_socket, envelope);
	found_client->second->lastSendMessageTime = current_time();
}

Envelope<::google::protobuf::Message> ServerConnector::receive()
{
	auto msg = recv_client_message(m_socket);
	auto found_client = m_client_map.find(msg.from);

	// new client connected
	if (found_client == m_client_map.end())
		m_client_map[msg.from] = new Client(msg.from);

	m_client_map[msg.from]->lastReceivedMessageTime = current_time();

	return msg;
}