#ifndef JOB_H
#define JOB_H

class IJob
{
public:
	virtual ~IJob() { }
	virtual void execute() = 0;
};

#endif // !JOB_H