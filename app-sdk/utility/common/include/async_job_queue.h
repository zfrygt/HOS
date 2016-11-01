#ifndef ASYNC_JOB_QUEUE_H
#define ASYNC_JOB_QUEUE_H

#include <memory>
#include <tbb/concurrent_queue.h>
#include <tbb/tbb_thread.h>
#include <assert.h>

template <typename Job>
class AsyncJobQueue
{
public:
	explicit AsyncJobQueue();
	~AsyncJobQueue();

	void add_job(std::shared_ptr<Job> job);
	typename tbb::concurrent_bounded_queue<std::shared_ptr<Job>>::size_type job_count() const { return m_job_queue->size(); }
protected:
	void start();
	void stop();

private:
	using job_queue = tbb::concurrent_bounded_queue<std::shared_ptr<Job>>;
	job_queue* m_job_queue;
	tbb::tbb_thread* m_job_thread;
	volatile bool m_started;
};

template <typename Job>
AsyncJobQueue<Job>::AsyncJobQueue():
m_job_queue(new job_queue),
m_job_thread(nullptr),
m_started(false)
{
	m_job_queue->set_capacity(100);
	start();
}

template <typename Job>
AsyncJobQueue<Job>::~AsyncJobQueue()
{
	stop();
}

template <typename Job>
void AsyncJobQueue<Job>::add_job(std::shared_ptr<Job> job)
{
	m_job_queue->push(std::move(job));
}

template <typename Job>
void AsyncJobQueue<Job>::start()
{
	if (!m_started && m_job_thread == nullptr)
	{
		m_started = true;

		m_job_thread = new tbb::tbb_thread([](AsyncJobQueue<Job>* queue)
		{
			auto job_queue = queue->m_job_queue;
			assert(job_queue != nullptr);

			while (queue->m_started)
			{
				std::shared_ptr<Job> job;
				job_queue->pop(job);
				if (!job) break;
				job->execute();
				job.reset();
			}
		}, this);
	}
}

template <typename Job>
void AsyncJobQueue<Job>::stop()
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
#endif // !ASYNC_JOB_QUEUE_H