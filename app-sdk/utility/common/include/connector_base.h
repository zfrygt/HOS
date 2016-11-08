#ifndef CONNECTOR_BASE_H
#define CONNECTOR_BASE_H

#include <stdint.h>
#include <common_macros.h>
#include <memory>
#include <string>
#include <envelope.h>
#include <hos_protocol.pb.h>

class IReceivePolicy;

namespace spdlog {
	class logger;
}

class COMMON_EXPORT ConnectorBase
{
public:
	ConnectorBase(IReceivePolicy* receive_policy, std::shared_ptr<spdlog::logger> logger, const char* uri);
	virtual ~ConnectorBase();
	virtual void poll(long timeout);
	virtual void start() = 0;
	virtual void init() = 0;
	virtual void destroy() = 0;
	virtual void send(Envelope<::google::protobuf::Message>* envelope) = 0;
	virtual Envelope<::google::protobuf::Message> receive() = 0;

	ConnectorBase(const ConnectorBase& other) = delete;
	ConnectorBase(ConnectorBase&& other) = delete;
	ConnectorBase& operator=(const ConnectorBase& other) = delete;
	ConnectorBase& operator=(ConnectorBase&& other) = delete;

protected:
	IReceivePolicy* m_receive_policy;
	int64_t m_lastSendMessageTime;
	int64_t m_lastReceivedMessageTime;
	void* m_context;
	void* m_socket;
	std::string m_uri;
	std::shared_ptr<spdlog::logger> m_logger;
};

#endif // !CONNECTOR_BASE_H