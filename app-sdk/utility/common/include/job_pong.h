#ifndef JOB_PONG_H
#define JOB_PONG_H

#include <job.h>

class ModuleConnector;

class COMMON_EXPORT JobPong : public IJob
{
public:
	explicit JobPong(ModuleConnector* module_connector);
	~JobPong();

	void execute() override;

private:
	ModuleConnector* m_module_connector;
};

#endif // !JOB_QUEUE_H