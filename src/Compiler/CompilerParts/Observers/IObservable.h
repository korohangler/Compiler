#pragma once
#include "IObserver.h"

template<class T, typename Y>
class IObservable
{
public:
	virtual void RegisterListener(T* observer) { m_observers.push_back(observer); }
	virtual void UnRegisterListener(T* observer) { m_observers.erase(std::find(m_observers.begin(), m_observers.end(), observer)); }

	virtual void Notify(Y data) { for (auto& observer : m_observers) observer->Notify(data); }
protected:
	std::vector<T*> m_observers;
};
