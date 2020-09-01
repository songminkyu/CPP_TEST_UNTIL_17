#pragma once
#include <utility>


using namespace std;
class LocalStaticSingleton
{
private:
	LocalStaticSingleton() = default;
	LocalStaticSingleton(const LocalStaticSingleton& other);
	~LocalStaticSingleton() = default;
public:
	static LocalStaticSingleton* GetInstance()
	{
		static LocalStaticSingleton ins;
		return &ins;
	}
};

template < typename T >
class CSAbstractDynamicSingleton
{
protected:
	CSAbstractDynamicSingleton() = default;
	virtual ~CSAbstractDynamicSingleton() = default;
public:
	static T * GetInstance()
	{
		if (m_pInstance == NULL)
			m_pInstance = new T;
		return m_pInstance;
	};

	static void DestroyInstance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	};

private:
	static T * m_pInstance;
};


//사용법
//template <class T> T* CSAbstractVarialdicDynamicSingleton<T>::m_pInstance = nullptr;
template <class T>
class CSAbstractVarialdicDynamicSingleton {
public:
	template<typename... Args>
	static T* GetInstance(Args&&... args) {
		if (!_instance) {
			_instance = new T(std::forward<Args>(args)...);
		}
		return _instance;
	}
	static void destroy() {
		if (_instance) {
			delete _instance;
			_instance = nullptr;
		}
	}

protected:
	CSAbstractVarialdicDynamicSingleton() = default;
	CSAbstractVarialdicDynamicSingleton(CSAbstractVarialdicDynamicSingleton const&) = default;
	CSAbstractVarialdicDynamicSingleton& operator=(CSAbstractVarialdicDynamicSingleton const&) { return *this; }
private:
	static T* _instance;

};
//사용법
//template <class T> T* CSAbstractVarialdicDynamicSingleton<T>::_instance = nullptr;


