#include <job_pong.h>
#include <hos_protocol.pb.h>
#include <module_connector.h>

JobPong::JobPong(ModuleConnector* module_connector) :
m_module_connector(module_connector)
{
	assert(m_module_connector != nullptr);
}

JobPong::~JobPong()
{
}

void JobPong::execute()
{
	assert(m_module_connector != nullptr);
	auto client_message = std::make_unique<ClientMessage>();
	client_message->set_type(ClientMessage::Pong);
	ProtobufMessageEnvelope envelope(std::move(client_message));
	m_module_connector->send(&envelope);
}