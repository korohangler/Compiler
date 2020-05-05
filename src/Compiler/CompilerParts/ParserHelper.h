#pragma once

#include "StageOutputStructs.h"

class __declspec(dllexport) ParserHelper
{
public:

	[[nodiscard]] static std::shared_ptr<AbstractTreeNode> CreateNewNodeFromToken(const Token& token);
};
