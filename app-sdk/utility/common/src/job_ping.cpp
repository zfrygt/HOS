#include <job_ping.h>
#include <hos_protocol.pb.h>
#include <server_connector.h>

JobPing::JobPing(ServerConnector* server_connector, const std::string& client_name) :
m_server_connector(server_connector),
m_client_name(std::move(client_name))
{
	assert(m_server_connector != nullptr);
	assert(!m_client_name.empty());
}

JobPing::~JobPing()
{
}

void JobPing::execute()
{
	assert(m_server_connector != nullptr);
	auto server_message = std::make_unique<ServerMessage>();
	server_message->set_type(ServerMessage_Type_Ping);
	ProtobufMessageEnvelope envelope(std::move(server_message), m_client_name);
	m_server_connector->send(&envelope);
}