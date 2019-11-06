#pragma once
template <typename T>
class CSingleton
{
protected:
	static T* inst;

public:
	static T* GetSingletonClass()
	{
		if (!inst)
			inst = new T();
		return inst;
	}

	static void ReleaseSingletonClass()
	{
		SAFE_DELETE(inst);
	}

public:
	CSingleton() {}
	virtual ~CSingleton() {}
};

template <typename T>
T* CSingleton<T>::inst = nullptr;

#define SINGLETON_GET(p)		p::GetSingletonClass()
#define SINGLETON_RELEASE(p)	p::ReleaseSingletonClass()
