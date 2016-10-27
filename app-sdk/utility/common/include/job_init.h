#ifndef JOB_INIT_H
#define JOB_INIT_H

#include <job.h>

class Connector;

class JobInit : public IJob
{
public:
	explicit JobInit(Connector* connector);
	~JobInit();

	void execute() override;

private:
	Connector* m_connector;
};

#endif // !JOB_QUEUE_H