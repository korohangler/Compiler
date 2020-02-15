#pragma once

class IStage
{
public:

	virtual ~IStage() {}
	
	virtual void DoStage(const std::wistream& inputStream, std::wostream& outputStream) = 0;

	virtual std::wstring GetStageName() = 0;
};
