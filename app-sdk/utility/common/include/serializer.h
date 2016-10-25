#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <assert.h>
#include <memory>

class NonCopyable
{
public:
	NonCopyable() { }
	virtual ~NonCopyable() { }

private:
	NonCopyable(const NonCopyable& other) = delete;
	NonCopyable(NonCopyable&& other) = delete;
	NonCopyable& operator=(const NonCopyable& other) = delete;
	NonCopyable& operator=(NonCopyable&& other) = delete;
};

class SerializedObject : NonCopyable
{
public:
	explicit SerializedObject(void* buf, uint64_t size) : 
		m_buf(buf),
		m_size(size)
	{
		
	}
	~SerializedObject()
	{
		free(m_buf);
		m_buf = nullptr;
		m_size = 0;
	}

	inline void* get_buf() const { return m_buf; }
	inline uint64_t get_size() const { return m_size; }

private:
	void* m_buf;
	uint64_t m_size;
};

class Serializer
{
public:
	template<typename T>
	std::unique_ptr<SerializedObject> serialize(T* t)
	{
		assert(t != nullptr);
		auto size = t->ByteSize();

		assert(size > 0);

		auto buf = malloc(size);
		t->SerializeToArray(buf, size);

		return std::make_unique<SerializedObject>(buf, size);
	}
};

#endif