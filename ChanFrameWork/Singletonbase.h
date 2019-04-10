#pragma once
/****************************************************************************
## Singletonbase ##
dynamicSingleton이 아니므로 직접 생성 및 해제 해주어야 함. 
./System/Window/Window.h의 private CreateManager에서 해줘야함
*****************************************************************************/
template <typename T>
class Singletonbase
{
protected:
	static T* mInstance;

	Singletonbase() {};
	virtual ~Singletonbase() {};
public:
	static T* Get(void);
	static void Create(void);
	static void Delete(void);
};

template <typename T>
T* Singletonbase<T>::mInstance = nullptr;

template <typename T>
T* Singletonbase<T>::Get(void)
{
	return Singletonbase<T>::mInstance;
}

template<typename T>
inline void Singletonbase<T>::Create(void)
{
	if (Singletonbase<T>::mInstance == nullptr)
	{
		Singletonbase<T>::mInstance = new T;
	}
}

template<typename T>
inline void Singletonbase<T>::Delete(void)
{
	if (Singletonbase<T>::mInstance)
	{
		delete Singletonbase<T>::mInstance;
		Singletonbase<T>::mInstance = nullptr;
	}
}



