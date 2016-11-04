#include <receive_strategy_queue.h>
#include <assert.h>
#include <hos_protocol.pb.h>

QueueStrategy::QueueStrategy(tbb::concurrent_bounded_queue<std::shared_ptr<ServerMessage>>* queue):
m_queue(queue)
{
	assert(m_queue != nullptr);
}

void QueueStrategy::operator()(std::unique_ptr<ServerMessage>&& server_message)
{
	m_queue->push(move(server_message));
}