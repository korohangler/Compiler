#pragma once

namespace std { class string_view; }

class IStage
{
public:

	virtual void doStage(std::string_view pathToFile) = 0;
};
