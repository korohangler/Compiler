#pragma once
#include "CompilerParts/IdentificatorTable.h"
#include "IStage.h"
#include "CompilerParts/Observers/IObserver.h"
#include "CompilerParts/StageOutputStructs.h"

class Let;
class ExpressionStatement;
class If;
class While;
class Function;

class __declspec(dllexport) CodeGeneratorStage : public IStage, public IObserver<std::pair<std::shared_ptr<AbstractTreeNode>, std::shared_ptr<IdentificatorTable>>>
{
public:
	/// IStage override
	void DoStage() override {}

	[[nodiscard]] std::wstring GetStageName() override { return L"CodeGenerator"; }
	///

	/// INewParserTreeObserver override
	void Notify(std::pair<std::shared_ptr<AbstractTreeNode>, std::shared_ptr<IdentificatorTable>> data) override;
	///

private:

	enum class Registers
	{
		AX,
		BX,
		CX,
		DX
	};

	void TranslateNode(std::shared_ptr<AbstractTreeNode> node);

	void TranslateLet(std::shared_ptr<Let> node);
	void TranslateExpressionStatement(std::shared_ptr<ExpressionStatement> node);
	void TranslateIfElement(std::shared_ptr<If> node);
	void TranslateWhileElement(std::shared_ptr<While> node);
	void TranslateFunction(std::shared_ptr<Function> node);
	
	std::shared_ptr<IdentificatorTable> m_table;
};
