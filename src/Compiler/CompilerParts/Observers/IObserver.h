#pragma once

template<class T>
class __declspec(dllexport) IObserver
{
public:
	virtual ~IObserver() = default;
	
	virtual void Notify(T data) = 0;
};