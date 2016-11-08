#ifndef RECEIVE_POLICY_BASE_H
#define RECEIVE_POLICY_BASE_H

#include <common_macros.h>
#include <envelope.h>
#include <google/protobuf/message.h>

using EnvelopeType = Envelope<::google::protobuf::Message>;

class COMMON_EXPORT IReceivePolicy{
public:
	virtual ~IReceivePolicy() { }
	virtual void operator()(std::shared_ptr<EnvelopeType>) = 0;
};

#endif