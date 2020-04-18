#pragma once

class IStage
{
public:
	IStage() = default;

	virtual ~IStage() = default;
	
	virtual void DoStage() = 0;

	virtual std::wstring GetStageName() = 0;
};
