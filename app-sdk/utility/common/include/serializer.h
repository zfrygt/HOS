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

#if defined(_WIN32) && defined(_MSC_VER)
        auto so = std::make_unique<SerializedObject>(size);
#else
        std::unique_ptr<SerializedObject> so(new SerializedObject(size));
#endif
		t->SerializeToArray(so->get_buf(), so->get_size());
		return so;
	}

	template<typename T>
	static std::unique_ptr<T> deserialize(SerializedObject* t)
	{
        assert(t != nullptr);
#if defined(_WIN32) && defined(_MSC_VER)
        auto temp = std::make_unique<T>();
#else
        std::unique_ptr<T> temp(new T);
#endif
		auto success = temp->ParseFromArray(t->get_buf(), t->get_size());
		assert(success);
		return temp;
	}
};

#endif
