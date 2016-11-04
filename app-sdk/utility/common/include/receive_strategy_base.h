#ifndef STRATEGY_BASE_H
#define STRATEGY_BASE_H

#include <memory>
#include <macros.h>

class ServerMessage;

class COMMON_EXPORT IReceiveStrategy{
public:
	virtual ~IReceiveStrategy() { }
	virtual void operator()(std::unique_ptr<ServerMessage>&&) = 0;
};

#endif