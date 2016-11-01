#ifndef SERVER_H
#define SERVER_H

#include <macros.h>
#include <string>
#include <memory>
#include <unordered_map>

class Client;
class ServerMessage;
class ClientMessage;
class IJob;

template<typename T>
class AsyncJobQueue;

class COMMON_EXPORT Server
{
	friend class IJob;
public:
	explicit Server(const char* uri);
	virtual ~Server();
	void start();
	void stop();
	void send(const std::string& client_name, const ServerMessage* server_message);
	void heartbeat(long timeout);

protected:
	Server(const Server& other) = delete;
	Server(Server&& other) = delete;
	Server& operator=(const Server& other) = delete;
	Server& operator=(Server&& other) = delete;

	std::unique_ptr<ClientMessage> receive(Client** sender_client);
	void send(Client* client, const ServerMessage* server_message);
	void on_receive();

private:
	void* m_context;
	void* m_socket;
	volatile bool m_started;
	std::unordered_map<std::string, Client*> m_client_map;
	AsyncJobQueue<IJob>* m_job_queue;
};

#endif
