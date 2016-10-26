#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <assert.h>
#include <memory>

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

		auto so = std::make_unique<SerializedObject>(size);
		t->SerializeToArray(so->get_buf(), so->get_size());
		return so;
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