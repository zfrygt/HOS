#ifndef JOB_PING_H
#define JOB_PING_H

#include <job.h>
#include "server.h"

class Server;

class JobPing : public IJob
{
public:
	explicit JobPing(Server* server, const std::string& client_name);
	~JobPing();

	void execute() override;

private:
	Server* m_server;
	std::string m_client_name;
};

#endif // !JOB_QUEUE_H