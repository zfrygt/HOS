#ifndef JOB_H
#define JOB_H

#include <common_macros.h>

class COMMON_EXPORT IJob
{
public:
	virtual ~IJob() { }
	virtual void execute() = 0;
};

#endif // !JOB_H