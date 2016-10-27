#ifndef JOB_PONG_H
#define JOB_PONG_H

#include <job.h>

class Connector;

class JobPong : public IJob
{
public:
	explicit JobPong(Connector* connector);
	~JobPong();

	void execute() override;

private:
	Connector* m_connector;
};

#endif // !JOB_QUEUE_H