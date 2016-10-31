#ifndef JOB_PING_H
#define JOB_PING_H

#include <job.h>
#include "server.h"

class Server;
class Client;

class JobPing : public IJob
{
public:
	explicit JobPing(Server* server, Client* client);
	~JobPing();

	void execute() override;

private:
	Server* m_server;
	Client* m_client;
};

#endif // !JOB_QUEUE_H