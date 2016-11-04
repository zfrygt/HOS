#ifndef MODULE_CONNECTOR_H
#define MODULE_CONNECTOR_H

#include <macros.h>
#include <string>
#include <stdint.h>
#include <memory>
#include <utils.h>

class ServerMessage;
class ClientMessage;
class IStrategy;

// ModuleConnector class that abstracts the heartbeating and other communication details.
class COMMON_EXPORT ModuleConnector : no_copy_move
{
public:
	explicit ModuleConnector(IStrategy* receive_strategy, const char* uri, const char* module_name);
	virtual ~ModuleConnector();	
	void connect();
	bool poll(long timeout);
	std::unique_ptr<ServerMessage> receive();
	void send(const ClientMessage* client_message);

protected:
	void destroy();
	void init();
	void reconnect();

private:
	void* m_context;
	void* m_socket;
	std::string m_uri;
	std::string m_module_name;
	int64_t m_lastSendMessageTime;
	int64_t m_lastReceivedMessageTime;
	IStrategy* m_on_receive_func;
	bool m_connected;
};

#endif
