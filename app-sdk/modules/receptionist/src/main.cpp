#include <module_connector.h>
#include <utils.h>
#include <tbb/concurrent_queue.h>
#include <strategy_queue.h>
#include <job.h>
#include <async_job_queue.h>
#include <job_pong.h>

using spServerMessage = std::shared_ptr<ServerMessage>;
const int MAX_JOB_COUNT = 100;

int main()
{
	AsyncJobQueue<IJob, MAX_JOB_COUNT> q;

	tbb::concurrent_bounded_queue<spServerMessage> queue;
	queue.set_capacity(MAX_JOB_COUNT);

	IStrategy *strategy = new QueueStrategy(&queue);

	ModuleConnector module_base(strategy, "tcp://localhost:5555", "receptionist");
	
	module_base.connect();

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
				q.add_job(std::make_shared<JobPong>(&module_base));
				break;
			case Pong: break;
			case Init: break;
			default: break;
			}
		}
	}

	delete strategy;

	return 0;
}