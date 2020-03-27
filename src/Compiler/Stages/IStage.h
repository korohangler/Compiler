#pragma once

class IStage
{
public:

	virtual ~IStage() {}
	
	virtual void DoStage() = 0;

	virtual std::wstring GetStageName() = 0;
};
