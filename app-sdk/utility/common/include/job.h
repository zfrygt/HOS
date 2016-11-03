#ifndef JOB_H
#define JOB_H

#include <macros.h>

class COMMON_EXPORT IJob
{
public:
	virtual ~IJob() { }
	virtual void execute() = 0;
};

#endif // !JOB_H