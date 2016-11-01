#ifndef CLIENT_H

#include <string>
#include <atomic>

class Client
{
public:
	explicit Client(const std::string& client_name) :
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

#endif // !CLIENT_H
