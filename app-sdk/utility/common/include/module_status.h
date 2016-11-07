#ifndef MODULE_STATUS_H
#define MODULE_STATUS_H

#include <common_macros.h>

class COMMON_EXPORT HardwareStatus
{
public:
	float get_cpu_usage();
	float get_disk_usage();
	float get_mem_usage();

private:
	float execute_command(const char* command);
};

#endif // !MODULE_STATUS_H
