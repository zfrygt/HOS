#ifndef JOB_CONTEXT_H
#define JOB_CONTEXT_H

#include <assert.h>

class JobContext
{
public:
	explicit JobContext(void* socket) : 
		m_socket(socket)
	{
		assert(m_socket != nullptr);
	}

	~JobContext() { }

	inline void* socket() const { return m_socket; }
private:
	void* m_socket;
};

#endif // !JOB_CONTEXT_H