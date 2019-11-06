#pragma once
template <typename SingletonClass>
class Singleton
{
private:
	static SingletonClass* m_instance;

public:
	static SingletonClass* GetSingleton()
	{
		if (!m_instance)
			m_instance = new SingletonClass();
		return m_instance;
	}

	static void ReleaseSingleton()
	{
		if (m_instance)
			delete m_instance;
	}

public:
	Singleton() {}
	~Singleton() {}
};

#define GetSingleton(c)		(c::GetSingleton())
#define ReleaseSingleton(c) (c::ReleaseSingleton())

template <typename SingletonClass>
SingletonClass* Singleton<SingletonClass>::m_instance = nullptr;
