#ifndef SERIALIZED_OBJECT_H
#define SERIALIZED_OBJECT_H

#include <assert.h>

class SerializedObject
{
public:
	explicit SerializedObject(uint64_t size) : 
		m_size(size)
	{
		assert(size > 0);
		m_buf = malloc(m_size);
		assert(m_buf != nullptr);
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

	inline void copyFrom(void* source_buffer)
	{
		assert(source_buffer != nullptr);
		memcpy(m_buf, source_buffer, m_size);
		assert(m_buf != nullptr);
	}

	inline void* get_buf() { return m_buf; }
	inline uint64_t get_size() const { return m_size; }

private:
	void* m_buf;
	uint64_t m_size;
};

#endif