#include <server_connector.h>
#include <iostream>
#include <spdlog/spdlog.h>
#include <receive_policy_queue.h>
#include <hos_protocol.pb.h>
#include <envelope.h>

using EnvelopType = Envelope<::google::protobuf::Message>;

int main()
{
	tbb::concurrent_bounded_queue<std::shared_ptr<EnvelopType>> message_queue;

	QueuePolicy queue_policy(&message_queue);

	ServerConnector connector(&queue_policy, "tcp://*:5555", spdlog::stdout_color_mt("supervisor"));

	connector.start();

	while (true)
	{
		std::shared_ptr<EnvelopType> envelope;
		message_queue.pop(envelope);

		if (envelope->payload)
		{
			auto message = static_cast<ClientMessage*>(envelope->payload.get());

			switch (message->type())
			{
			case ClientMessage_Type_Success: break;
			case ClientMessage_Type_Init:
			{
				auto server_message = std::make_unique<ServerMessage>();
				server_message->set_type(ServerMessage_Type_Success);
				Envelope<::google::protobuf::Message> env(std::move(server_message), envelope->to);
				connector.send(&env);
			}
				break;
			case ClientMessage_Type_Pong: break;
			case ClientMessage_Type_BadRequest: break;
			case ClientMessage_Type_CaptureInfo: break;
			case ClientMessage_Type_HostInfo: break;
			case ClientMessage_Type_Snapshot: break;
			case ClientMessage_Type_HostStatus: break;
			default: break;
			}
		}
		else
			break;
	}

	/*
	//Client *client = nullptr;
	//auto msg = receive(&client);
	//assert(client != nullptr);

	//switch (msg->type())
	//{
	//case ClientMessage::Pong:
	//	
	//	break;
	//case ClientMessage::Init:
	//{
	//	{
	//		ServerMessage server_message;
	//		server_message.set_type(ServerMessage_Type_Success);
	//		send(client, &server_message);
	//		m_logger->info("[{}] connected", client->get_client_name());
	//	}
	//		{
	//			ServerMessage server_message;
	//			server_message.set_type(ServerMessage_Type_HostInfo);
	//			send(client, &server_message);
	//		}
	//}
	//break;
	//case ClientMessage::HostInfo:
	//{
	//	assert(msg->has_host_info());

	//	auto host_info = msg->host_info();
	//	m_logger->info("cpu count: {}, total ram: {}, total disk: {}", host_info.total_cpu(), host_info.total_ram(), host_info.total_disk());
	//}
	//break;
	//default: break;
	//}
	*/

	std::cin.get();

	return 0;
}