#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <macros.h>
#include <string>
#include <time.h>

// Connector class that abstracts the heartbeating and other communication details.
class COMMON_EXPORT Connector
{
public:
	Connector(void* ctx, const std::string& uri, const std::string& module_name);
	virtual ~Connector();
	void start();
	void heartbeat();

protected:
	virtual void send(int commandId, const void* data, size_t size);
	virtual void receive();

private:
	void* m_context;
	void* m_socket;
	std::string m_uri;
	std::string m_module_name;
	clock_t m_lastSendMessageTime;
	clock_t m_lastReceivedMessageTime;
};

#endif