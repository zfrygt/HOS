#include "module_status.h"

float HardwareStatus::get_cpu_usage()
{
	return execute_command<float>("top -bn 1 | awk 'NR>7{s+=$9} END {print s}'");
}

float HardwareStatus::get_dsk_usage()
{
	return execute_command<float>("df -k | grep /dev/root | awk '{print $3/$2*100}'");
}

float HardwareStatus::get_mem_usage()
{
	return execute_command<float>("free -t -m | grep Mem: | awk '{print ($3-$6-$7)/$2*100}'");
}

uint32_t HardwareStatus::get_total_cpu()
{
	return execute_command<uint32_t>("cat /proc/cpuinfo | grep processor | wc -l");
}

uint32_t HardwareStatus::get_total_mem()
{
	return execute_command<uint32_t>("free -t -m | grep Mem: | awk '{print $2}'");
}

uint32_t HardwareStatus::get_total_dsk()
{
	return execute_command<uint32_t>("df -m | grep /dev/root | awk '{print $2}'");
}