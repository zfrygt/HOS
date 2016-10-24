#include "connector.h"
#include <zmq.h>
#include <assert.h>

Connector::Connector(void* ctx, const std::string& uri, const std::string& module_name):
m_context(ctx),
m_uri(std::move(uri)),
m_module_name(std::move(module_name)),
m_lastSendMessageTime(-1),
m_lastReceivedMessageTime(-1)
{
	m_socket = zmq_socket(m_context, ZMQ_DEALER);
	auto linger = 0;
	zmq_setsockopt(m_socket, ZMQ_LINGER, &linger, sizeof(linger)); // close cagirildiktan sonra beklemeden socket'i kapat.

	std::string sp("tPw$8v!-!O}kL[5VRvT<yg&NbWolkR=eVQC5Z8X6");
	zmq_setsockopt(m_socket, ZMQ_CURVE_SERVERKEY, sp.c_str(), sp.length());
	zmq_setsockopt(m_socket, ZMQ_IDENTITY, m_module_name.c_str(), m_module_name.length());

	char public_key[41];
	char secret_key[41];

	zmq_curve_keypair(public_key, secret_key);

	zmq_setsockopt(m_socket, ZMQ_CURVE_PUBLICKEY, public_key, sizeof(public_key));
	zmq_setsockopt(m_socket, ZMQ_CURVE_SECRETKEY, secret_key, sizeof(secret_key));

	zmq_connect(m_socket, m_uri.c_str());
}

Connector::~Connector()
{
	zmq_disconnect(m_socket, m_uri.c_str());
	zmq_close(m_socket);
}

void Connector::start()
{

}

void Connector::heartbeat()
{

}

void Connector::send(int commandId, const void* data, size_t size)
{
	// Send empty frame
	int r1 = zmq_send(m_socket, nullptr, 0, ZMQ_SNDMORE);
	assert(r1 == 0);
	// Send resultIdentifier to track messages
	int r2 = zmq_send(m_socket, m_module_name.c_str(), m_module_name.length(), ZMQ_SNDMORE);
	assert(r2 == m_module_name.length());
	// Send command id
	int r3 = zmq_send(m_socket, &commandId, sizeof(commandId), size ? ZMQ_SNDMORE : 0);
	assert(r3 == sizeof(commandId));
	// Send data if any
	if (size)
	{
		int r4 = zmq_send(m_socket, data, size, 0);
		assert(r4 == size);
	}
	m_lastSendMessageTime = clock();
	if (m_lastReceivedMessageTime < 0)
		m_lastReceivedMessageTime = m_lastSendMessageTime;
}

void Connector::receive()
{

}