#pragma once
template <typename Class>
class Singleton
{
private:
	static Class* m_instance;

public:
	static Class* GetInstance()
	{
		if (!m_instance)
			m_instance = new Class();
		return m_instance;
	}

	static void ReleaseInstance()
	{
		if (m_instance)
		{
			delete m_instance;
			m_instance = nullptr;
		}
	}

public:
	Singleton() {}
	~Singleton() {}
};

template <typename Class>
Class* Singleton<Class>::m_instance = nullptr;

#define GetSingleton(c) (c::GetInstance())
#define ReleaseSingleton(c) (c::ReleaseInstance())
