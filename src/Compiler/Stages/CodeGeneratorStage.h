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
	CodeGeneratorStage(std::wstring directory) : m_constructor(std::make_shared<ASMConstructor>(std::move(directory))), m_jumpNameMarks(0){}
	
	/// IStage override
	void DoStage() override {}

	[[nodiscard]] std::wstring_view GetStageName() override { return L"CodeGenerator"; }
	///

	/// IObserver override
	void Notify(std::pair<std::shared_ptr<AbstractTreeNode>, std::shared_ptr<IdentificatorTable>> data) override;
	///

private:

	size_t m_jumpNameMarks;

	std::shared_ptr<ASMConstructor> m_constructor;

	void TranslateNode(const std::shared_ptr<AbstractTreeNode> node);

	void TranslateLet(const std::shared_ptr<Let> node);
	void TranslateExpressionStatement(const std::shared_ptr<ExpressionStatement> node);
	void TranslateIfElement(const std::shared_ptr<If> node);
	void TranslateWhileElement(const std::shared_ptr<While> node);
	void TranslateFunction(const std::shared_ptr<Function> node);

	void TranslateExpression(const std::shared_ptr<Expression> node, const std::shared_ptr<Identificator> whereToStore);
	void TranslateExpressionNode(const std::shared_ptr<AbstractTreeNode> node);

	void TranslateFunctionCall(const std::shared_ptr<ExpressionStatement> node);
	
	std::shared_ptr<IdentificatorTable> m_table;
};
