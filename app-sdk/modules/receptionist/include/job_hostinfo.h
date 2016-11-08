#ifndef JOB_HOST_INFO_H
#define JOB_HOST_INFO_H

#include <job.h>

class ModuleConnector;

class JobHostInfo : public IJob
{
public:
	explicit JobHostInfo(ModuleConnector* module_connector);

	~JobHostInfo()
	{
	}

	void execute() override;

private:
	ModuleConnector* m_module_connector;
};


#endif // !JOB_HOST_INFO_H
