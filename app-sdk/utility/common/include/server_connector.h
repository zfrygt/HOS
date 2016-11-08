#ifndef SERVER_CONNECTOR_H
#define SERVER_CONNECTOR_H

#include <common_macros.h>
#include <string>
#include <memory>
#include <unordered_map>
#include <connector_base.h>
#include <common_utils.h>

class Client;
class ServerMessage;
class ClientMessage;
class IJob;

template<typename T>
class Envelope;

namespace spdlog
{
	class logger;
}

class COMMON_EXPORT ServerConnector : ConnectorBase
{
	friend class IJob;
public:
	explicit ServerConnector(IReceivePolicy* receive_strategy, const char* uri, std::shared_ptr<spdlog::logger> logger);
	virtual ~ServerConnector();
	void start() override;
	void destroy() override;
	void poll(long timeout) override;
	void init() override;
	void send(ProtobufMessageEnvelope* envelope) override;
	ProtobufMessageEnvelope receive() override;

private:
	volatile bool m_started;
	std::unordered_map<std::string, Client*> m_client_map;
};

#endif
