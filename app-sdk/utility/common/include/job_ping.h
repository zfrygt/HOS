#ifndef JOB_PING_H
#define JOB_PING_H

#include <job.h>
#include <string>

class ServerConnector;

class JobPing : public IJob
{
public:
	explicit JobPing(ServerConnector* server_connector, const std::string& client_name);
	~JobPing();

	void execute() override;

private:
	ServerConnector* m_server_connector;
	std::string m_client_name;
};

#endif // !JOB_QUEUE_H