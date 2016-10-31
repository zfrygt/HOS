#include <job_ping.h>
#include <hos_protocol.pb.h>
#include <server.h>

JobPing::JobPing(Server* server, Client* client):
m_server(server),
m_client(client)
{
	assert(m_server != nullptr);
	assert(m_client != nullptr);
}

JobPing::~JobPing()
{
}

void JobPing::execute()
{
	assert(m_server != nullptr);
	ServerMessage server_message;
	server_message.set_type(Ping);
	m_server->send(m_client, &server_message);
}