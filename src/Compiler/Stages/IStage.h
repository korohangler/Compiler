#pragma once

class __declspec(dllexport) IStage
{
public:
	IStage() = default;

	virtual ~IStage() = default;
	
	virtual void DoStage() = 0;

	[[nodiscard]] virtual std::wstring_view GetStageName() = 0;
};
