#include <module_connector.h>
#include <zmq.h>
#include <hos_protocol.pb.h>
#include <iostream>
#include <strategy_base.h>

#define ZMQ_CHECK(x){int result = (x); if (result==1)printf("%d\n",zmq_errno()); assert(result!=-1);}

ModuleConnector::ModuleConnector(IStrategy* receive_strategy, const char* uri, const char* module_name) :
m_uri(uri),
m_module_name(module_name),
m_on_receive_func(receive_strategy)
{
	assert(!m_uri.empty());
	assert(!m_module_name.empty());

	init();
}

ModuleConnector::~ModuleConnector()
{
	destroy();
}

void ModuleConnector::poll(long timeout)
{
	zmq_pollitem_t items[] = {
		{ m_socket, 0, ZMQ_POLLIN, 0 }
	};
	zmq_poll(items, 1, timeout);
	if (items[0].revents & ZMQ_POLLIN){
		m_on_receive_func->operator()(move(receive()));
	}
	auto secondsSinceLastMessageReceived = current_time() - m_lastReceivedMessageTime;
	if (m_lastReceivedMessageTime >= 0 && secondsSinceLastMessageReceived > TIMEOUT_INTERVAL_IN_SECONDS){
		// Timeout all!
		std::cout << "reconnecting to server\n";
		reconnect();
	}
	return true;
}

void ModuleConnector::connect()
{
	assert(m_context != nullptr);
	assert(m_socket != nullptr);

	zmq_connect(m_socket, m_uri.c_str());
	ClientMessage msg;
	msg.set_type(Init);
	send(&msg);
	// wait for the response from the server
	m_connected = receive()->type() == Init;
}

std::unique_ptr<ServerMessage> ModuleConnector::receive()
{
	auto server_message = recv_server_message(m_socket);
	m_lastReceivedMessageTime = current_time();

	std::cout << "from server: " << MessageType_Name(server_message->type()) << "\n";

	return server_message;
}

void ModuleConnector::send(const ClientMessage* client_message)
{
	send_client_message(m_socket, client_message);

	m_lastSendMessageTime = current_time();

	if (m_lastReceivedMessageTime < 0)
		m_lastReceivedMessageTime = m_lastSendMessageTime;

	std::cout << "to server: " << MessageType_Name(client_message->type()) << "\n";
}

void ModuleConnector::init()
{
	m_context = zmq_ctx_new();
	assert(m_context != nullptr);
	m_socket = zmq_socket(m_context, ZMQ_DEALER);
	assert(m_socket != nullptr);
	m_connected = false;
	m_started = false;
	m_lastSendMessageTime = -1;
	m_lastReceivedMessageTime = -1;

	auto linger = 0;
	auto r = zmq_setsockopt(m_socket, ZMQ_LINGER, &linger, sizeof(linger)); // close cagirildiktan sonra beklemeden socket'i kapat.
	assert(r == 0);

	r = zmq_setsockopt(m_socket, ZMQ_IDENTITY, m_module_name.c_str(), m_module_name.length());
	assert(r == 0);

#ifdef USE_CURVE
	std::string sp("tPw$8v!-!O}kL[5VRvT<yg&NbWolkR=eVQC5Z8X6");
	r = zmq_setsockopt(m_socket, ZMQ_CURVE_SERVERKEY, sp.c_str(), sp.length());
	assert(r == 0);

	char public_key[41];
	char secret_key[41];

	r = zmq_curve_keypair(public_key, secret_key);
	assert(r == 0);

	r = zmq_setsockopt(m_socket, ZMQ_CURVE_PUBLICKEY, public_key, sizeof(public_key));
	assert(r == 0);
	r = zmq_setsockopt(m_socket, ZMQ_CURVE_SECRETKEY, secret_key, sizeof(secret_key));
	assert(r == 0);
#endif
}

void ModuleConnector::reconnect()
{
	destroy();
	init();
	connect();
}

void ModuleConnector::destroy()
{
	m_connected = false;
	m_started = false;

	if (m_socket != nullptr)
	{
		zmq_disconnect(m_socket, m_uri.c_str());
		zmq_close(m_socket);
		m_socket = nullptr;
	}

	if (m_context != nullptr)
	{
		zmq_ctx_destroy(m_context);
		m_context = nullptr;
	}
}