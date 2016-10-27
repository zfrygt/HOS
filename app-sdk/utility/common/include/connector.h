#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <macros.h>
#include <string>
#include <time.h>
#include <stdint.h>
#include <job.h>
#include <memory>

namespace tbb
{
	template<typename T>
	class cache_aligned_allocator;

	template<typename T, class A = cache_aligned_allocator<T>>
	class concurrent_bounded_queue;
}

class ServerMessage;
class ClientMessage;

// Connector class that abstracts the heartbeating and other communication details.
class COMMON_EXPORT Connector
{
public:
	Connector(const char* uri, const char* module_name);
	virtual ~Connector();
	std::unique_ptr<ServerMessage> receive();
	void send(const ClientMessage* client_message);
	void heartbeat(long timeout);

protected:
	Connector(const Connector& other) = delete;
	Connector(Connector&& other) = delete;
	Connector& operator=(const Connector& other) = delete;
	Connector& operator=(Connector&& other) = delete;

private:
	void* m_context;
	void* m_socket;
	char *m_uri;
	char *m_module_name;
	uint8_t m_uri_len, m_module_name_len;
	clock_t m_lastSendMessageTime;
	clock_t m_lastReceivedMessageTime;

	tbb::concurrent_bounded_queue<std::unique_ptr<IJob>>* m_job_queue;
};

#endif