#ifndef STRATEGY_QUEUE_H
#define STRATEGY_QUEUE_H

#include <strategy_base.h>
#include <tbb/concurrent_queue.h>

class COMMON_EXPORT QueueStrategy : public IStrategy{
	using Queue = tbb::concurrent_bounded_queue<std::shared_ptr<ServerMessage>>;
public:
	explicit QueueStrategy(Queue* queue);
	~QueueStrategy() { }
	void operator()(std::unique_ptr<ServerMessage>&&) override;

private:
	Queue* m_queue;
};

#endif