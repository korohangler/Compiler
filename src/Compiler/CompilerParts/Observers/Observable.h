#pragma once
#include "IObserver.h"

template<class T>
class Observable
{
public:
	virtual ~Observable() = default;
	
	virtual void RegisterListener(IObserver<T>* observer) { m_observers.push_back(observer); }
	virtual void UnRegisterListener(IObserver<T>* observer) { m_observers.erase(std::find(m_observers.begin(), m_observers.end(), observer)); }

	virtual void NotifyListeners(T data) { for (auto& observer : m_observers) observer->Notify(data); }
protected:
	std::vector<IObserver<T>*> m_observers;
};
