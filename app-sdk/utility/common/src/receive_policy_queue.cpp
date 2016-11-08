#include <receive_policy_queue.h>
#include <assert.h>
#include <hos_protocol.pb.h>

QueuePolicy::QueuePolicy(Queue* queue) :
m_queue(queue)
{
	assert(m_queue != nullptr);
}

void QueuePolicy::operator()(std::shared_ptr<ProtobufMessageEnvelope> msg)
{
	m_queue->push(std::move(msg));
}