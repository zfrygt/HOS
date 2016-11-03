#ifndef ASYNC_JOB_QUEUE_H
#define ASYNC_JOB_QUEUE_H

#include <memory>
#include <tbb/concurrent_queue.h>
#include <tbb/tbb_thread.h>
#include <assert.h>

template <typename Job, int N>
class AsyncJobQueue
{
	using spJob = std::shared_ptr<Job>;
	using job_queue = tbb::concurrent_bounded_queue<spJob>;

public:
	explicit AsyncJobQueue():
		m_job_queue(new job_queue),
		m_job_thread(nullptr),
		m_started(false)
	{
		m_job_queue->set_capacity(N);
		start();
	}

	~AsyncJobQueue()
	{
		stop();
	}

	void add_job(spJob&& job)
	{
		m_job_queue->push(std::forward<spJob>(job));
	}

	typename job_queue::size_type job_count() const { return m_job_queue->size(); }
protected:
	void start()
	{
		if (!m_started && m_job_thread == nullptr)
		{
			m_started = true;

			m_job_thread = new tbb::tbb_thread([](AsyncJobQueue<Job, N>* queue)
			{
				auto job_queue = queue->m_job_queue;
				assert(job_queue != nullptr);

				while (queue->m_started)
				{
					spJob job;
					job_queue->pop(job);
					if (!job) break;
					job->execute();
					job.reset();
				}
			}, this);
		}
	}

	void stop()
	{
		if (m_started && m_job_queue != nullptr && m_job_thread != nullptr)
		{
			m_started = false;
			m_job_queue->push(nullptr);
			m_job_thread->join();
			delete m_job_thread;
			m_job_thread = nullptr;

			assert(m_job_queue->empty());
		}
	}

private:
	job_queue* m_job_queue;
	tbb::tbb_thread* m_job_thread;
	volatile bool m_started;
};

#endif // !ASYNC_JOB_QUEUE_H