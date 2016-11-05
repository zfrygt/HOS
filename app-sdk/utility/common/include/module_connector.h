#ifndef MODULE_CONNECTOR_H
#define MODULE_CONNECTOR_H

#include <macros.h>
#include <string>
#include <stdint.h>
#include <memory>
#include <utils.h>

class ServerMessage;
class ClientMessage;
class IReceivePolicy;

namespace spdlog
{
	class logger;
}

// ModuleConnector class that abstracts the heartbeating and other communication details.
class COMMON_EXPORT ModuleConnector : no_copy_move
{
public:
	explicit ModuleConnector(IReceivePolicy* receive_strategy, std::shared_ptr<spdlog::logger> logger, const char* uri, const char* module_name);
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
	IReceivePolicy* m_on_receive_func;
	std::shared_ptr<spdlog::logger> m_logger;
	bool m_connected;
};

#endif
