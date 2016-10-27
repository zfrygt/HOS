#include <job_context.h>
#include <hos_protocol.pb.h>
#include <connector.h>
#include <iostream>
#include <job_init.h>

JobInit::JobInit(Connector* connector) :
m_connector(connector)
{
	assert(m_connector);
}

JobInit::~JobInit()
{
	std::cout << "Destructing JobInit\n";
}

void JobInit::execute()
{
	ClientMessage client_message;
	client_message.set_type(Init);
	m_connector->send(&client_message);
}