#include <job_pong.h>
#include <job_context.h>
#include <hos_protocol.pb.h>
#include <connector.h>
#include <iostream>

JobPong::JobPong(Connector* connector):
m_connector(connector)
{
	assert(m_connector);
}

JobPong::~JobPong()
{
	std::cout << "Destructing PongJob\n";
}

void JobPong::execute()
{
	ClientMessage client_message;
	client_message.set_type(Pong);
	m_connector->send(&client_message);
}