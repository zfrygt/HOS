#ifndef MODULE_CONNECTOR_H
#define MODULE_CONNECTOR_H

#include <common_macros.h>
#include <string>
#include <memory>
#include <common_utils.h>
#include <connector_base.h>

namespace spdlog
{
	class logger;
}

// ModuleConnector class that abstracts the heartbeating and other communication details.
class COMMON_EXPORT ModuleConnector : ConnectorBase
{
public:
	explicit ModuleConnector(IReceivePolicy* receive_strategy, std::shared_ptr<spdlog::logger> logger, const char* uri, const char* module_name);
	virtual ~ModuleConnector();	
	void poll(long timeout) override;
	void start() override;
	void destroy() override;

	void send(Envelope<::google::protobuf::Message>* envelope) override;
	Envelope<::google::protobuf::Message> receive() override;

protected:
	void init() override;
	void reconnect();

private:
	std::string m_module_name;
	bool m_connected;
};

#endif
