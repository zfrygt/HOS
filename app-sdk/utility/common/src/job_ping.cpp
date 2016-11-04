#include <job_ping.h>
#include <hos_protocol.pb.h>
#include <server.h>

JobPing::JobPing(Server* server, const std::string& client_name):
m_server(server),
m_client_name(std::move(client_name))
{
	assert(m_server != nullptr);
	assert(!m_client_name.empty());
}

JobPing::~JobPing()
{
}

void JobPing::execute()
{
	assert(m_server_connector != nullptr);
	ServerMessage server_message;
	server_message.set_type(Ping);
	m_server_connector->send(m_client_name, &server_message);
	std::cout << "ping to [" << m_client_name << "]\n";
}