#ifndef SERVER_CONNECTOR_H
#define SERVER_CONNECTOR_H

#include <common_macros.h>
#include <string>
#include <memory>
#include <unordered_map>
#include <common_utils.h>

class Client;
class ServerMessage;
class ClientMessage;
class IJob;

namespace spdlog
{
	class logger;
}

template<typename T, int>
class AsyncJobQueue;

class COMMON_EXPORT ServerConnector : no_copy_move
{
	friend class IJob;
public:
	explicit ServerConnector(const char* uri, std::shared_ptr<spdlog::logger> logger);
	virtual ~ServerConnector();
	void start();
	void stop();
	void send(const std::string& client_name, const ServerMessage* server_message);
	void heartbeat(long timeout);
	std::unordered_map<std::string, Client*>::size_type client_count() const { return m_client_map.size(); }

protected:
	std::unique_ptr<ClientMessage> receive(Client** sender_client);
	void send(Client* client, const ServerMessage* server_message);
	void on_receive();

private:
	void* m_context;
	void* m_socket;
	volatile bool m_started;
	std::unordered_map<std::string, Client*> m_client_map;
	AsyncJobQueue<IJob, 100>* m_job_queue;
	std::shared_ptr<spdlog::logger> m_logger;
};

#endif
