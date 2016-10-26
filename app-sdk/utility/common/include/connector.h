#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <macros.h>
#include <string>
#include <time.h>
#include <stdint.h>

// Connector class that abstracts the heartbeating and other communication details.
class COMMON_EXPORT Connector
{
public:
	Connector(const char* uri, const char* module_name);
	virtual ~Connector();
	void receive();
	void send(const void* data, uint64_t size);
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
};

#endif