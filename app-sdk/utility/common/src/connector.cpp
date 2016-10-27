#include "connector.h"
#include <zmq.h>
#include <assert.h>
#include <hos_protocol.pb.h>
#include <serializer.h>
#include <tbb/concurrent_queue.h>
#include <utils.h>

#define HEARTHBEAT_INTERVAL_IN_SECONDS 5 
#define TIMEOUT_INTERVAL_IN_SECONDS 15
#define TIMEOUT_CHECK_INTERVAL_IN_SECONDS 2

#define ZMQ_CHECK(x){int result = (x); if (result==1)printf("%d\n",zmq_errno()); assert(result!=-1);}

Connector::Connector(const char* uri, const char* module_name) :
m_lastSendMessageTime(-1),
m_lastReceivedMessageTime(-1)
{
	m_uri_len = strlen(uri);
	assert(m_uri_len > 0);

	m_module_name_len = strlen(module_name);
	assert(m_module_name_len > 0);

	m_uri = static_cast<char*>(malloc(m_uri_len + 1));
	strcpy(m_uri, uri);

	m_module_name = static_cast<char*>(malloc(m_module_name_len + 1));
	strcpy(m_module_name, module_name);

	m_context = zmq_ctx_new();
	assert(m_context != nullptr);
	m_socket = zmq_socket(m_context, ZMQ_DEALER);
	assert(m_socket != nullptr);

	auto linger = 0;
	auto r = zmq_setsockopt(m_socket, ZMQ_LINGER, &linger, sizeof(linger)); // close cagirildiktan sonra beklemeden socket'i kapat.
	assert(r == 0);

	//std::string sp("tPw$8v!-!O}kL[5VRvT<yg&NbWolkR=eVQC5Z8X6");
	//r = zmq_setsockopt(m_socket, ZMQ_CURVE_SERVERKEY, sp.c_str(), sp.length());
	//assert(r == 0);
	r = zmq_setsockopt(m_socket, ZMQ_IDENTITY, m_module_name, m_module_name_len);
	assert(r == 0);

	//char public_key[41];
	//char secret_key[41];

	//r = zmq_curve_keypair(public_key, secret_key);
	//assert(r == 0);

	//r = zmq_setsockopt(m_socket, ZMQ_CURVE_PUBLICKEY, public_key, sizeof(public_key));
	//assert(r == 0);
	//r = zmq_setsockopt(m_socket, ZMQ_CURVE_SECRETKEY, secret_key, sizeof(secret_key));
	//assert(r == 0);

	zmq_connect(m_socket, m_uri);

	ClientMessage client_message;
	client_message.set_type(Init);
	send(&client_message);
}

Connector::~Connector()
{
	if (m_uri)
	{
		free(m_uri);
		m_uri = nullptr;
	}
	if (m_module_name)
	{
		free(m_module_name);
		m_module_name = nullptr;
	}
	zmq_disconnect(m_socket, m_uri);
	zmq_close(m_socket);
	zmq_ctx_destroy(m_context);
}

void Connector::heartbeat(long timeout)
{

	zmq_pollitem_t items[] = {
		{ m_socket, 0, ZMQ_POLLIN, 0 }
	};
	zmq_poll(items, 1, timeout);
	if (items[0].revents & ZMQ_POLLIN){
		receive();
	}
	auto currentTime = clock();
	auto timeSinceLastMessageSend = currentTime - m_lastSendMessageTime;
	auto timeSinceLastMessageReceived = currentTime - m_lastReceivedMessageTime;
	auto secondsSinceLastMessageSend = timeSinceLastMessageSend / CLOCKS_PER_SEC;
	auto secondsSinceLastMessageReceived = timeSinceLastMessageReceived / CLOCKS_PER_SEC;
	if (m_lastReceivedMessageTime >= 0 && secondsSinceLastMessageReceived > TIMEOUT_INTERVAL_IN_SECONDS){
		// Timeout all!

		// TODO It might be a good idea to destroy socket inorder to celar send buffer so previous messages will not be send accidentaly
	}
	if (m_lastSendMessageTime >= 0 && secondsSinceLastMessageSend > HEARTHBEAT_INTERVAL_IN_SECONDS){
		ClientMessage response;
		response.set_type(Pong);
		send(&response);
	}
}

std::unique_ptr<ServerMessage> Connector::receive()
{
	auto server_message = recv_server_message(m_socket);
	m_lastReceivedMessageTime = clock();
	return server_message;
}

void Connector::send(const ClientMessage* client_message)
{
	send_client_message(m_socket, client_message);

	m_lastSendMessageTime = clock();

	if (m_lastReceivedMessageTime < 0)
		m_lastReceivedMessageTime = m_lastSendMessageTime;
}