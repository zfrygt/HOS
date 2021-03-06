#ifndef QUEUE_POLICY_H
#define QUEUE_POLICY_H

#include <receive_policy_base.h>
#include <tbb/concurrent_queue.h>

class COMMON_EXPORT QueuePolicy : public IReceivePolicy{
	using Queue = tbb::concurrent_bounded_queue<std::shared_ptr<ServerMessage>>;
public:
	explicit QueuePolicy() {}
	explicit QueuePolicy(Queue* queue);
	~QueuePolicy() { }
	void operator()(std::unique_ptr<ServerMessage>&&) override;

private:
	Queue* m_queue;
};

#endif