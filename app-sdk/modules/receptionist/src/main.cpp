#include <module_connector.h>
#include <utils.h>
#include <tbb/concurrent_queue.h>
#include <job.h>
#include <async_job_queue.h>
#include <job_pong.h>
#include <receive_policy_queue.h>
#include <module_connector_policy.h>
#include <spdlog/spdlog.h>
#include <capture.h>
#include <iostream>

const auto WIDTH = 640;
const auto HEIGHT = 480;

using spServerMessage = std::shared_ptr<ServerMessage>;
const int MAX_JOB_COUNT = 100;

using QueueType = tbb::concurrent_bounded_queue<spServerMessage>;

int main()
{
	ICaptureFactory* captureFactory = new WebcamCaptureFactory();
	auto capture = captureFactory->createFactory("/dev/video0");

	CaptureSettings set(WIDTH, HEIGHT, 3, 5, AV_CODEC_ID_MJPEG);

	capture->init(&set);

	Decoder decoder(WIDTH, HEIGHT);

	if (!decoder.setup(capture)) {
		std::cout << "cannot initialize decoder" << std::endl;
		exit(EXIT_FAILURE);
	}

	AsyncJobQueue<IJob, MAX_JOB_COUNT> q;

	QueueType queue;
	queue.set_capacity(MAX_JOB_COUNT);

	ModuleConnectorTemp<QueuePolicy, QueueType*> module(spdlog::stdout_color_mt("console"), "tcp://192.168.1.5:5555", &queue);

	while (true)
	{
		spServerMessage message;
		queue.pop(message);

		if (!message) break;

		if (message)
		{
			switch (message->type())
			{
			case Ping:
				q.add_job(std::make_shared<JobPong>(module.get_connector()));
				break;
			case Pong: break;
			case Init: break;
			default: break;
			}
		}
	}

	return 0;
}