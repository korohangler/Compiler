#pragma once
#include "CompilerParts/IdentificatorTable.h"
#include "IStage.h"
#include "CompilerParts/Observers/Observable.h"
#include "StageOutputStructs/AbstractTreeNode.h"
#include "CompilerParts/ASMConstructor.h"

class Let;
class ExpressionStatement;
class If;
class While;
class Function;
class Expression;
class Identificator;

class __declspec(dllexport) CodeGeneratorStage : public IStage,
public IObserver<std::pair<std::shared_ptr<AbstractTreeNode>, std::shared_ptr<IdentificatorTable>>>,
public Observable<std::shared_ptr<ASMConstructor>>
{
public:
	CodeGeneratorStage(std::wstring directory) : m_constructor(std::make_shared<ASMConstructor>(std::move(directory))) {}
	
	/// IStage override
	void DoStage() override {}

	[[nodiscard]] std::wstring_view GetStageName() override { return L"CodeGenerator"; }
	///

	/// INewParserTreeObserver override
	void Notify(std::pair<std::shared_ptr<AbstractTreeNode>, std::shared_ptr<IdentificatorTable>> data) override;
	///

private:

	std::shared_ptr<ASMConstructor> m_constructor;

	void TranslateNode(std::shared_ptr<AbstractTreeNode> node);

	void TranslateLet(std::shared_ptr<Let> node);
	void TranslateExpressionStatement(std::shared_ptr<ExpressionStatement> node);
	void TranslateIfElement(std::shared_ptr<If> node);
	void TranslateWhileElement(std::shared_ptr<While> node);
	void TranslateFunction(std::shared_ptr<Function> node);

	// result of this will be saved in eax register
	void TranslateExpression(std::shared_ptr<Expression> node, std::shared_ptr<Identificator> whereToStore);
	void TranslateExpressionNode(std::shared_ptr<AbstractTreeNode> node);
	
	std::shared_ptr<IdentificatorTable> m_table;
};
