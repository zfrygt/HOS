#ifndef SERVER_H
#define SERVER_H

#include <macros.h>
#include <string>
#include <stdint.h>
#include <job.h>
#include <memory>
#include <unordered_map>

#ifdef _WIN32
namespace tbb
{
	template<typename T>
	class cache_aligned_allocator;

	template<typename T, class A = cache_aligned_allocator<T>>
	class concurrent_bounded_queue;
}
#else
    #include <tbb/concurrent_queue.h>
#endif

class ServerMessage;
class ClientMessage;

namespace std
{
	template<typename T>
	class future;
}

class Client
{
public:
	explicit Client(const std::string& client_name):
		lastSendMessageTime(-1),
		lastReceivedMessageTime(-1),
		m_client_name(std::move(client_name))
	{
		
	}

	inline std::string get_client_name() const { return m_client_name; }

	Client(const Client& other) = delete;
	Client(Client&& other) = delete;
	Client& operator=(const Client& other) = delete;
	Client& operator=(Client&& other) = delete;

	int64_t lastSendMessageTime;
	int64_t lastReceivedMessageTime;

private:
	std::string m_client_name;
};

class COMMON_EXPORT Server
{
public:
	explicit Server(const char* uri);
	virtual ~Server();
	void heartbeat(long timeout);
	void start();
	std::unique_ptr<ClientMessage> receive(Client** sender_client);
	void send(Client* client, const ServerMessage* server_message);
	bool has_client() const { return !m_client_map.empty(); }

protected:
	Server(const Server& other) = delete;
	Server(Server&& other) = delete;
	Server& operator=(const Server& other) = delete;
	Server& operator=(Server&& other) = delete;

private:
	void* m_context;
	void* m_socket;
	bool m_started;
	using job_queue = tbb::concurrent_bounded_queue<std::shared_ptr<IJob>>;
	job_queue* m_job_queue;
	std::future<void>* m_job_thread;
	std::unordered_map<std::string, Client*> m_client_map;
};

#endif
