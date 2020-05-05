#pragma once
#include "IObserver.h"

template<class T>
class IObservable
{
public:
	virtual void RegisterListener(IObserver<T>* observer) { m_observers.push_back(observer); }
	virtual void UnRegisterListener(IObserver<T>* observer) { m_observers.erase(std::find(m_observers.begin(), m_observers.end(), observer)); }

	virtual void Notify(T data) { for (auto& observer : m_observers) observer->Notify(data); }
protected:
	std::vector<IObserver<T>*> m_observers;
};
