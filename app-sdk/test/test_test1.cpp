#include <zmq.h>
#include <connector.h>
#include <assert.h>

int main()
{
	auto ctx = zmq_ctx_new();

	auto connector = new Connector(ctx, "tcp://localhost:5555", "test");

	connector->heartbeat();

	delete connector;

	zmq_ctx_destroy(ctx);

	return 0;
}