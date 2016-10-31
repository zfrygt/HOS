#include "connector.h"
#include <zmq.h>
#include <assert.h>
#include <hos_protocol.pb.h>
#include <tbb/concurrent_queue.h>
#include <utils.h>
#include <future>
#include <iostream>
#include <job_pong.h>

#define ZMQ_CHECK(x){int result = (x); if (result==1)printf("%d\n",zmq_errno()); assert(result!=-1);}

Connector::Connector(const char* uri, const char* module_name) :
m_timeout(false),
m_lastSendMessageTime(-1),
m_lastReceivedMessageTime(-1),
m_started(false),
m_job_queue(new job_queue),
m_job_thread(nullptr)
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

	r = zmq_setsockopt(m_socket, ZMQ_IDENTITY, m_module_name, m_module_name_len);
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
	if (m_job_queue)
	{
		m_job_queue->push(nullptr);
		// join the job thread first!
		m_job_thread->get();
		assert(m_job_queue->empty());
		delete m_job_queue;
	}
	zmq_disconnect(m_socket, m_uri);
	zmq_close(m_socket);
	zmq_ctx_destroy(m_context);
}

void Connector::heartbeat(long timeout)
{
	std::unique_ptr<ServerMessage> msg;
	zmq_pollitem_t items[] = {
		{ m_socket, 0, ZMQ_POLLIN, 0 }
	};
	zmq_poll(items, 1, timeout);
	if (items[0].revents & ZMQ_POLLIN){
		msg = receive();
		if (msg)
		{
			switch (msg->type())
			{
			case Ping:
				m_job_queue->push(move(std::make_shared<JobPong>(this)));
				break;
			default: break;
			}
		}
	}
	auto secondsSinceLastMessageReceived = current_time() - m_lastReceivedMessageTime;
    if (m_lastReceivedMessageTime >= 0 && secondsSinceLastMessageReceived > TIMEOUT_INTERVAL_IN_SECONDS){
        // Timeout all!
		m_timeout = true;
    }
}

void Connector::connect()
{
	zmq_connect(m_socket, m_uri);
	ClientMessage msg;
	msg.set_type(Init);
	send(&msg);

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

std::unique_ptr<ServerMessage> Connector::receive()
{
	auto server_message = recv_server_message(m_socket);
	m_lastReceivedMessageTime = current_time();

	std::cout << "from server: " << MessageType_Name(server_message->type()) << "\n";

	return server_message;
}

void Connector::send(const ClientMessage* client_message)
{
	send_client_message(m_socket, client_message);

	m_lastSendMessageTime = current_time();

	if (m_lastReceivedMessageTime < 0)
		m_lastReceivedMessageTime = m_lastSendMessageTime;

	std::cout << "to server: " << MessageType_Name(client_message->type()) << "\n";
}
