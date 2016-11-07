#include "module_status.h"
#include <stdio.h>
#include <cmath>
#include <memory>

struct FileCloser
{
	void operator()(FILE* desc){
		_pclose(desc);
	}
};

float HardwareStatus::get_cpu_usage()
{
	return execute_command("top -bn 1 | awk 'NR>7{s+=$9} END {print s}'");
}

float HardwareStatus::get_disk_usage()
{
	return execute_command("df -k | grep /dev/root | awk '{print $3/$2*100}'");
}

float HardwareStatus::get_mem_usage()
{
	return execute_command("free -t -m | grep Mem: | awk '{print ($3-$6-$7)/$2*100}'");
}

float HardwareStatus::execute_command(const char* command)
{
	std::unique_ptr<FILE, FileCloser> fd(_popen(command, "r"));

	if (!fd) return 0.f;

	char buffer[256] = { 0 };

	while (fgets(buffer, sizeof buffer, fd.get()) != nullptr);

	return static_cast<float>(atof(buffer));
}