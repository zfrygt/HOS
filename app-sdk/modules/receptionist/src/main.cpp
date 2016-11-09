#include <module_connector.h>
#include <tbb/concurrent_queue.h>
#include <job.h>
#include <async_job_queue.h>
#include <job_pong.h>
#include <receive_policy_queue.h>
#include <module_connector_policy.h>
#include <spdlog/spdlog.h>
#include <capture_utils.h>
#include <job_hostinfo.h>
#include <sstream>

const int MAX_JOB_COUNT = 100;

using spEnvelope = std::shared_ptr<ProtobufMessageEnvelope>;
using MessageQueueType = tbb::concurrent_bounded_queue<spEnvelope>;

int main(int argc, char* argv[])
{
	if (argc != 2) return -1;

	create_if_not_exist("logs");

	AsyncJobQueue<IJob, MAX_JOB_COUNT> q;

	MessageQueueType message_queue;
	message_queue.set_capacity(MAX_JOB_COUNT);

	std::stringstream ss;
	ss << "tcp://" << argv[1] << ":5555";

	ModuleConnectorTemp<QueuePolicy, MessageQueueType*> module(spdlog::stdout_color_mt("console"), ss.str(), &message_queue);

	while (true)
	{
		spEnvelope envelope;
		message_queue.pop(envelope);

		if (envelope->payload)
		{
			auto message = static_cast<ServerMessage*>(envelope->payload.get());

			switch (message->type())
			{
			case ServerMessage_Type_Ping:
				q.add_job(std::make_shared<JobPong>(module.get_connector()));
				break;
			case ServerMessage_Type_HostInfo:
				q.add_job(std::make_shared<JobHostInfo>(module.get_connector()));
				break;
			default: break;
			}
		}
		else
			break;
	}

	return 0;
}