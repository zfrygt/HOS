#include <connector_base.h>
#include <zmq.h>
#include <receive_policy_base.h>

ConnectorBase::ConnectorBase(IReceivePolicy* receive_policy, std::shared_ptr<spdlog::logger> logger, const char* uri) :
m_receive_policy(receive_policy),
m_lastSendMessageTime(-1),
m_lastReceivedMessageTime(-1),
m_uri(uri),
m_logger(std::move(logger))
{
}

ConnectorBase::~ConnectorBase()
{

}

void ConnectorBase::poll(long timeout)
{
	zmq_pollitem_t items[] = {
		{ m_socket, 0, ZMQ_POLLIN, 0 }
	};
	zmq_poll(items, 1, timeout);
	if (items[0].revents & ZMQ_POLLIN){
		m_receive_policy->operator()(std::make_shared<ProtobufMessageEnvelope>(receive()));
	}
}