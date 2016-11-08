#include <module_connector.h>
#include <zmq.h>
#include <hos_protocol.pb.h>
#include <spdlog/spdlog.h>
#include <receive_policy_base.h>

ModuleConnector::ModuleConnector(IReceivePolicy* receive_strategy, std::shared_ptr<spdlog::logger> logger, const char* uri, const char* module_name) :
ConnectorBase(receive_strategy, std::forward<std::shared_ptr<spdlog::logger>>(logger), uri),
m_module_name(module_name),
m_connected(false)
{
	assert(!m_uri.empty());
	assert(!m_module_name.empty());
}

ModuleConnector::~ModuleConnector()
{
	ModuleConnector::destroy();
}

void ModuleConnector::poll(long timeout)
{
	ConnectorBase::poll(timeout);

	auto secondsSinceLastMessageReceived = current_time() - m_lastReceivedMessageTime;
	if (m_lastReceivedMessageTime >= 0 && secondsSinceLastMessageReceived > TIMEOUT_INTERVAL_IN_SECONDS){
		// Timeout all!
		if (m_logger)
			m_logger->info("reconnecting to server...", "");
		reconnect();
	}
}

void ModuleConnector::start()
{
	this->init();

	assert(m_context != nullptr);
	assert(m_socket != nullptr);

	zmq_connect(m_socket, m_uri.c_str());
	auto msg = std::make_unique<ClientMessage>();
	msg->set_type(ClientMessage::Init);

	Envelope<::google::protobuf::Message> env(std::move(msg));
	send(&env);

	// wait for the response from the server
	m_connected = static_cast<ClientMessage*>(receive().payload.get())->type() == ServerMessage_Type_Success;

	while (m_connected)
		poll(25);
}

void ModuleConnector::init()
{
	m_context = zmq_ctx_new();
	assert(m_context != nullptr);
	m_socket = zmq_socket(m_context, ZMQ_DEALER);
	assert(m_socket != nullptr);
	m_connected = false;
	m_lastSendMessageTime = -1;
	m_lastReceivedMessageTime = -1;

	auto linger = 0;
	auto r = zmq_setsockopt(m_socket, ZMQ_LINGER, &linger, sizeof(linger)); // close cagirildiktan sonra beklemeden socket'i kapat.
	assert(r == 0);

	r = zmq_setsockopt(m_socket, ZMQ_IDENTITY, m_module_name.c_str(), m_module_name.length());
	assert(r == 0);

#ifdef USE_CURVE
	std::string sp("tPw$8v!-!O}kL[5VRvT<yg&NbWolkR=eVQC5Z8X6");
	r = zmq_setsockopt(m_socket, ZMQ_CURVE_SERVERKEY, sp.c_str(), sp.length());
	assert(r == 0);

	char public_key[41];
	char secret_key[41];

	r = zmq_curve_keypair(public_key, secret_key);
	assert(r == 0);

	r = zmq_setsockopt(m_socket, ZMQ_CURVE_PUBLICKEY, public_key, sizeof(public_key));
	assert(r == 0);
	r = zmq_setsockopt(m_socket, ZMQ_CURVE_SECRETKEY, secret_key, sizeof(secret_key));
	assert(r == 0);
#endif
}

void ModuleConnector::reconnect()
{
	destroy();
	start();
}

void ModuleConnector::destroy()
{
	m_connected = false;

	if (m_socket != nullptr)
	{
		zmq_disconnect(m_socket, m_uri.c_str());
		zmq_close(m_socket);
		m_socket = nullptr;
	}

	if (m_context != nullptr)
	{
		zmq_ctx_destroy(m_context);
		m_context = nullptr;
	}
}

void ModuleConnector::send(Envelope<::google::protobuf::Message>* envelope)
{
	send_client_message(m_socket, envelope);

	m_lastSendMessageTime = current_time();

	if (m_lastReceivedMessageTime < 0)
		m_lastReceivedMessageTime = m_lastSendMessageTime;

	auto payload = static_cast<ClientMessage*>(envelope->payload.get());

	if (m_logger)
		m_logger->info("to server: {}..", ClientMessage::Type_Name(payload->type()));
}

Envelope<::google::protobuf::Message> ModuleConnector::receive()
{
	auto server_message = recv_server_message(m_socket);
	m_lastReceivedMessageTime = current_time();

	auto payload = static_cast<ServerMessage*>(server_message.payload.get());

	if (m_logger)
		m_logger->info("from server: {}..", ServerMessage::Type_Name(payload->type()));

	return server_message;
}