#ifndef JOB_H
#define JOB_H

class JobContext;

class IJob
{
public:
	virtual ~IJob() { }
	virtual void execute(JobContext* ctx) = 0;
};

#endif // !JOB_H