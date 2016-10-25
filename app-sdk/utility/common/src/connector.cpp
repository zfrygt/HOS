#include "connector.h"
#include <zmq.h>
#include <assert.h>
#include <hos_protocol.pb.h>

#define HEARTHBEAT_INTERVAL_IN_SECONDS 5 
#define TIMEOUT_INTERVAL_IN_SECONDS 15
#define TIMEOUT_CHECK_INTERVAL_IN_SECONDS 2

#define ZMQ_CHECK(x){int result = (x); if (result==1)printf("%d\n",zmq_errno()); assert(result!=-1);}

Connector::Connector(const char* uri, const char* module_name):
m_lastSendMessageTime(-1),
m_lastReceivedMessageTime(-1)
{
	memset(m_uri, 0, MAX_LEN);
	memset(m_module_name, 0, MAX_LEN);

	m_uri_len = strlen(uri);
	assert(m_uri_len > 0);
	m_module_name_len = strlen(module_name);
	assert(m_module_name_len > 0);

	memcpy(m_uri, uri, m_uri_len);
	memcpy(m_module_name, module_name, m_module_name_len);

	m_context = zmq_ctx_new();
	assert(m_context != nullptr);
	m_socket = zmq_socket(m_context, ZMQ_DEALER);
	assert(m_socket != nullptr);

	auto linger = 0;
	auto r = zmq_setsockopt(m_socket, ZMQ_LINGER, &linger, sizeof(linger)); // close cagirildiktan sonra beklemeden socket'i kapat.

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
}

Connector::~Connector()
{
	zmq_disconnect(m_socket, m_uri);
	zmq_close(m_socket);
	zmq_ctx_destroy(m_context);
}

void Connector::start()
{

}

void Connector::heartbeat(uint32_t timeout)
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
		Request req;
		req.set_request(Request_RequestType_Ping);
		send(&req);
	}
}

void Connector::receive()
{
	zmq_msg_t msg;
	int responseCode;
	int more;
	// Read NULL frame
	{
		zmq_msg_init(&msg);
		ZMQ_CHECK(zmq_recvmsg(m_socket, &msg, 0));
		more = zmq_msg_more(&msg);
		assert(more);
		zmq_msg_close(&msg);
	}
	// Read result identifier
	{
		zmq_msg_init(&msg);
		ZMQ_CHECK(zmq_recvmsg(m_socket, &msg, 0));
		more = zmq_msg_more(&msg);
		assert(more == 0);
		assert(strncmp("murat", static_cast<char*>(zmq_msg_data(&msg)), zmq_msg_size(&msg)) == 0);
		zmq_msg_close(&msg);
	}
	
	// Discard remaining!
	while (more){
		zmq_msg_init(&msg);
		ZMQ_CHECK(zmq_recvmsg(m_socket, &msg, 0));
		more = zmq_msg_more(&msg);
		zmq_msg_close(&msg);
	}
	m_lastReceivedMessageTime = clock();
}

void Connector::send(const void* data, uint64_t size)
{
	assert(data != nullptr);
	assert(size > 0);

	// Send empty frame
	int r1 = zmq_send(m_socket, nullptr, 0, ZMQ_SNDMORE);
	assert(r1 == 0);

	int r4 = zmq_send(m_socket, data, size, 0);
	assert(r4 == size);

	m_lastSendMessageTime = clock();

	if (m_lastReceivedMessageTime < 0)
		m_lastReceivedMessageTime = m_lastSendMessageTime;
}

void Connector::send(Request* request)
{
	assert(request != nullptr);


}