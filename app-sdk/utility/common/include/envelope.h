#ifndef ENVELOPE_H
#define ENVELOPE_H

#include <memory>
#include <string>
#include <utility>

template<typename T>
class Envelope
{
public:
	explicit Envelope() { }

	explicit Envelope(std::unique_ptr<T>&& payload, const std::string& to = std::string(), const std::string& from = std::string())
		: payload(std::forward<std::unique_ptr<T>>(payload)),
		  to(move(to)),
		  from(move(from))
	{
	}

	std::unique_ptr<T> payload;
	std::string to;
	std::string from;

	Envelope(const Envelope& other) = delete;
	Envelope& operator=(const Envelope& other) = delete;

	Envelope(Envelope&& other)
		: payload(std::move(other.payload)),
		  to(std::move(other.to)),
		  from(std::move(other.from))
	{
	}

	Envelope& operator=(Envelope&& other)
	{
		if (this == &other)
			return *this;
		payload = std::move(other.payload);
		to = std::move(other.to);
		from = std::move(other.from);
		return *this;
	}
};

#endif // !ENVELOPE_H
