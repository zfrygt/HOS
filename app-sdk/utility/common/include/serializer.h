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
		if (m_buf)
		{
			free(m_buf);
			m_buf = nullptr;
		}
		m_size = 0;
	}

	SerializedObject(const SerializedObject& other) = delete;
	SerializedObject(SerializedObject&& other) = delete;
	SerializedObject& operator=(const SerializedObject& other) = delete;
	SerializedObject& operator=(SerializedObject&& other) = delete;

	inline void* get_buf() { return m_buf; }
	inline uint64_t get_size() const { return m_size; }

private:
	void* m_buf;
	uint64_t m_size;
};

class Serializer
{
public:
	template<typename T>
	static std::unique_ptr<SerializedObject> serialize(T* t)
	{
		auto size = t->ByteSize();
		assert(size > 0);
		auto buf = malloc(size);
		t->SerializeToArray(buf, size);
		return std::make_unique<SerializedObject>(buf, size);
	}

	template<typename T>
	static std::unique_ptr<T> deserialize(std::unique_ptr<SerializedObject> t)
	{
		auto temp = std::make_unique<T>();
		auto success = temp->ParseFromArray(t->get_buf(), t->get_size());
		assert(success);
		return temp;
	}
};

#endif