#pragma once

template <typename T>
class Singleton
{
private:
	static T* m_instance;

public:
	static T* SingletonGet()
	{
		if (!m_instance)
			m_instance = new T();
		return m_instance;
	}

	static void SingletonRelease()
	{
		if (m_instance)
		{
			delete m_instance;
			m_instance = nullptr;
		}
	}

public:
	Singleton() {}
	virtual ~Singleton() {}
};

template <typename T>
T* Singleton<T>::m_instance = nullptr;


#define SingletonInstance(s)	(s::SingletonGet())
#define SingletonRelease(s)		(s::SingletonRelease())
