#include <module_connector.h>
#include <tbb/concurrent_queue.h>
#include <job.h>
#include <async_job_queue.h>
#include <job_pong.h>
#include <receive_policy_queue.h>
#include <module_connector_policy.h>
#include <spdlog/spdlog.h>
#include <video_capture.h>
#include <capture_utils.h>

const auto WIDTH = 640;
const auto HEIGHT = 480;


using spServerMessage = std::shared_ptr<ServerMessage>;
const int MAX_JOB_COUNT = 100;

using QueueType = tbb::concurrent_bounded_queue<spServerMessage>;

int main()
{
	create_if_not_exist("logs");

	//if (!dir_exists("logs")) _mkdir("logs");

	//VideoCapture<Webcam> capture("/dev/video0");
	//capture.init();
	//capture.start([](void* ptr)
	//{
	//	
	//});

	AsyncJobQueue<IJob, MAX_JOB_COUNT> q;

	QueueType message_queue;
	message_queue.set_capacity(MAX_JOB_COUNT);

	ModuleConnectorTemp<QueuePolicy, QueueType*> module(spdlog::stdout_color_mt("console"), "tcp://localhost:5555", &message_queue);

	while (true)
	{
		spServerMessage message;
		message_queue.pop(message);

		if (!message) break;

		if (message)
		{
			switch (message->type())
			{
			case ServerMessage_Type_Ping:
				q.add_job(std::make_shared<JobPong>(module.get_connector()));
				break;
			default: break;
			}
		}
	}

	return 0;
}