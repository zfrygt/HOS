#ifndef MODULE_CONNECTOR_POLICIES_H
#define MODULE_CONNECTOR_POLICIES_H

#include <future>
#include <module_connector.h>

template<typename ReceivePolicy, typename ...Args>
class ModuleConnectorTemp : ReceivePolicy
{
public:
	explicit ModuleConnectorTemp(std::shared_ptr<spdlog::logger>&& logger, const std::string url, Args&&... args) :
		m_receive_policy(new ReceivePolicy(std::forward<Args>(args)...))
	{
#if defined(_WIN32) && defined(_MSC_VER)
		m_connector = std::make_unique<ModuleConnector>(m_receive_policy, std::forward<std::shared_ptr<spdlog::logger>>(logger), url.c_str(), "receptionist");
#else
		m_connector = std::unique_ptr<ModuleConnector>(new ModuleConnector(m_receive_policy, std::forward<std::shared_ptr<spdlog::logger>>(logger), url.c_str(), "receptionist"));
#endif
		connect();
	}

	~ModuleConnectorTemp()
	{
		m_future.get();
	}

	inline ModuleConnector* get_connector() const { return m_connector.get(); }

protected:
	void connect()
	{
		auto future = std::async(std::launch::async, [this](){
			m_connector->connect();
		});

		m_future = std::move(future);
	}

private:
	ReceivePolicy* m_receive_policy;
	std::unique_ptr<ModuleConnector> m_connector;
	std::future<void> m_future;
};

#endif