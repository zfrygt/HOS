#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <memory>
#include <serializedobject.h>

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
	static std::unique_ptr<T> deserialize(SerializedObject* t)
	{
		assert(t != nullptr);
		auto temp = std::make_unique<T>();
		auto success = temp->ParseFromArray(t->get_buf(), t->get_size());
		assert(success);
		return temp;
	}
};

#endif