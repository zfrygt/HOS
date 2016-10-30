#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <macros.h>
#include <string>
#include <time.h>
#include <stdint.h>
#include <job.h>
#include <memory>


#ifdef _WIN32
namespace tbb
{
	template<typename T>
	class cache_aligned_allocator;

	template<typename T, class A = cache_aligned_allocator<T>>
	class concurrent_bounded_queue;
}
#else
    #include <tbb/concurrent_queue.h>
#endif

class ServerMessage;
class ClientMessage;

namespace std
{
	template<typename T>
	class future;
}

// Connector class that abstracts the heartbeating and other communication details.
class COMMON_EXPORT Connector
{
public:
	Connector(const char* uri, const char* module_name);
	virtual ~Connector();
	void heartbeat(long timeout);
	void start();
	std::unique_ptr<ServerMessage> receive();
	void send(const ClientMessage* client_message);

	inline bool timeout() const { return m_timeout; }

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
	bool m_timeout;
	uint8_t m_uri_len, m_module_name_len;
	int64_t m_lastSendMessageTime;
	int64_t m_lastReceivedMessageTime;
	bool m_started;
	using job_queue = tbb::concurrent_bounded_queue<std::shared_ptr<IJob>>;
	job_queue* m_job_queue;
	std::future<void>* m_job_thread;
};

#endif
