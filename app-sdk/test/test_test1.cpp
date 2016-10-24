#include <zmq.h>
#include <connector.h>
#include <assert.h>
//#include <hos_protocol.pb.h>

int main()
{
	auto context = zmq_ctx_new();
	assert(context != nullptr);

	auto server_socket = zmq_socket(context, ZMQ_ROUTER);
	assert(context != nullptr);
	
	zmq_bind(server_socket, "tcp://*.5555");

	auto connector = new Connector("tcp://localhost:5555", "test");
	connector->heartbeat(25);
	delete connector;

	zmq_close(server_socket);
	zmq_ctx_destroy(context);

	return 0;
}