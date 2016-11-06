#ifndef RECEIVE_POLICY_BASE_H
#define RECEIVE_POLICY_BASE_H

#include <memory>
#include <common_macros.h>

class ServerMessage;

class COMMON_EXPORT IReceivePolicy{
public:
	virtual ~IReceivePolicy() { }
	virtual void operator()(std::unique_ptr<ServerMessage>&&) = 0;
};

#endif