#include <job_ping.h>
#include <hos_protocol.pb.h>
#include <server_connector.h>
#include <iostream>

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
	ServerMessage server_message;
	server_message.set_type(ServerMessage_Type_Ping);
	m_server_connector->send(m_client_name, &server_message);
	std::cout << "ping to [" << m_client_name << "]\n";
}