#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <macros.h>
#include <string>
#include <stdint.h>
#include <memory>

class ServerMessage;
class ClientMessage;
class IJob;

template<typename T>
class AsyncJobQueue;

namespace std
{
	template<typename T>
	class future;
}

// Connector class that abstracts the heartbeating and other communication details.
class COMMON_EXPORT Connector
{
public:
	explicit Connector(const char* uri, const char* module_name);
	virtual ~Connector();
	void heartbeat(long timeout);
	void connect();
	std::unique_ptr<ServerMessage> receive();
	void send(const ClientMessage* client_message);

	inline bool connected() const { return m_connected; }

protected:
	void destroy();
	void init();

	Connector(const Connector& other) = delete;
	Connector(Connector&& other) = delete;
	Connector& operator=(const Connector& other) = delete;
	Connector& operator=(Connector&& other) = delete;

private:
	void* m_context;
	void* m_socket;
	volatile bool m_connected;
	volatile bool m_started;
	std::string m_uri;
	std::string m_module_name;
	int64_t m_lastSendMessageTime;
	int64_t m_lastReceivedMessageTime;
	AsyncJobQueue<IJob>* m_job_queue;
};

#endif
