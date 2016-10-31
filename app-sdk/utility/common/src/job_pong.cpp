#include <job_pong.h>
#include <hos_protocol.pb.h>
#include <connector.h>

JobPong::JobPong(Connector* connector):
m_connector(connector)
{
	assert(m_connector != nullptr);
}

JobPong::~JobPong()
{
}

void JobPong::execute()
{
	assert(m_connector != nullptr);
	ClientMessage client_message;
	client_message.set_type(Pong);
	m_connector->send(&client_message);
}