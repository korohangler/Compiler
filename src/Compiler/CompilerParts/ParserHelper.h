#pragma once

#include "StageOutputStructs.h"

class ParserHelper
{
public:

	static std::shared_ptr<AbstractTreeNode> CreateNewNodeFromToken(const Token& token);
};