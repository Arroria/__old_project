#pragma once

template <typename T>
class Singleton
{
private:
	static T* m_inst;

public:
	static T* SingletonGet()
	{
		if (!m_inst)
			m_inst = new T;
		return m_inst;
	}
	static void SingletonDelete()
	{
		SAFE_DELETE(m_inst);
	}

public:
	Singleton() {}
	virtual ~Singleton() {}
};

template <typename T>
T* Singleton<T>::m_inst = nullptr;

#define SingletonInstance(c)	(c::SingletonGet())
#define SingletonRelease(c)		(c::SingletonDelete())
