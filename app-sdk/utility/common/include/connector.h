#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <macros.h>
#include <string>
#include <time.h>
#include <stdint.h>

const uint32_t MAX_LEN = 80;

class Request;

// Connector class that abstracts the heartbeating and other communication details.
class COMMON_EXPORT Connector
{
public:
	Connector(const char* uri, const char* module_name);
	virtual ~Connector();
	void start();
	void heartbeat(uint32_t timeout);

private:
	Connector(const Connector& other) = delete;
	Connector(Connector&& other) = delete;
	Connector& operator=(const Connector& other) = delete;
	Connector& operator=(Connector&& other) = delete;

public:
	virtual void receive();
	virtual void send(Request* request, const void* data, size_t size);

private:
	void* m_context;
	void* m_socket;
	char m_uri[MAX_LEN];
	char m_module_name[MAX_LEN];
	uint8_t m_uri_len, m_module_name_len;
	clock_t m_lastSendMessageTime;
	clock_t m_lastReceivedMessageTime;
};

#endif