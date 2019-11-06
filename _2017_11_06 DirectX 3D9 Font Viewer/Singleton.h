#pragma once
template <typename SingletonClass>
class Singleton
{
private:
	static SingletonClass* m_singletonClass;

public:
	static SingletonClass* GetSingletonClass()
	{
		if (!m_singletonClass)
			m_singletonClass = new SingletonClass();
		return m_singletonClass;
	}

	static void ReleaseSingletonClass()
	{
		SAFE_DELETE(m_singletonClass);
	}

public:
	Singleton() {}
	virtual ~Singleton() {}
};

template <typename SingletonClass>
SingletonClass* Singleton<SingletonClass>::m_singletonClass = nullptr;

#define GetSingleton(c) Singleton<c>::GetSingletonClass()
#define ReleaseSingleton(c) Singleton<c>::ReleaseSingletonClass()
