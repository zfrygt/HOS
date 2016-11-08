#ifndef RECEIVE_POLICY_BASE_H
#define RECEIVE_POLICY_BASE_H

#include <common_macros.h>
#include "common_utils.h"

class COMMON_EXPORT IReceivePolicy{
public:
	virtual ~IReceivePolicy() { }
	virtual void operator()(std::shared_ptr<ProtobufMessageEnvelope>) = 0;
};

#endif