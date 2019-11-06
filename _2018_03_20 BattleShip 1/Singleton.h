#pragma once

template <typename toner>
class Singleton
{
private:
	static toner* m_instance;

public:
	static toner* GetSingleton()
	{
		if (!m_instance)
			m_instance = new toner;
		return m_instance;
	}
	static void DestroySingleton()
	{
		SAFE_DELETE( m_instance );
	}

public:
	Singleton() {}
	~Singleton() {}
};

template <typename toner>
toner* Singleton<toner>::m_instance = nullptr;

#define SingletonInstance(toner)	(toner::GetSingleton())
#define SingletonRelease(toner)		(toner::DestroySingleton())

