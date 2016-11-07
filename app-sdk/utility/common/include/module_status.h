#ifndef MODULE_STATUS_H
#define MODULE_STATUS_H

#include <common_macros.h>

class COMMON_EXPORT HardwareStatus
{
public:
	static float get_cpu_usage();
	static float get_disk_usage();
	static float get_mem_usage();

private:
	static float execute_command(const char* command);
};

#endif // !MODULE_STATUS_H
