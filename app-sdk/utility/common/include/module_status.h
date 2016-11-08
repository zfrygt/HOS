#ifndef MODULE_STATUS_H
#define MODULE_STATUS_H

#include <common_macros.h>
#include <stdint.h>
#include <memory>
#include <cctype>
#include <stdio.h>
#include <sstream>

struct FileCloser
{
	void operator()(FILE* desc){
#if defined(_WIN32) && defined(_MSC_VER)
		//_pclose(desc);
#else
		pclose(desc);
#endif
	}
};

class COMMON_EXPORT HardwareStatus
{
public:
	static float get_cpu_usage();
	static float get_dsk_usage();
	static float get_mem_usage();

	static uint32_t get_total_cpu();
	static uint32_t get_total_mem();
	static uint32_t get_total_dsk();

private:
	template<typename T>
	static T execute_command(const char* command);
};

template <typename T>
T HardwareStatus::execute_command(const char* command)
{
#if defined(_WIN32) && defined(_MSC_VER)
	return 0.f; // not implemented yet!
#else
	std::unique_ptr<FILE, FileCloser> fd(_popen(command, "r"));

	if (!fd) return 0.f;

	char buffer[80] = { 0 };

	while (fgets(buffer, sizeof buffer, fd.get()) != nullptr);

	std::stringstream ss(buffer);

	T retVal;

	ss >> retVal;

	return retVal;
#endif
}
#endif // !MODULE_STATUS_H
