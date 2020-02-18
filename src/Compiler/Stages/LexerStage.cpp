#include "stdafx.h"
#include "LexerStage.h"

LexerStage::LexerStage(std::wstring runningDirectory)
{
	std::wstring pathToConfig = runningDirectory + L"../../../data/Lexer/regexps.json";

	FILE* pFile;
	errno_t status = fopen_s(&pFile, Utils::StringConverter::WstrToStr(pathToConfig).c_str(), "rb");
	
	ASSERT2(pFile, "Unable to open Lexer config file!");

	char buffer[65536];
	rapidjson::FileReadStream is(pFile, buffer, sizeof(buffer));
	rapidjson::Document document;
	document.ParseStream<0, rapidjson::UTF8<>, rapidjson::FileReadStream>(is);

	InitRules(document);

	if (pFile != nullptr) fclose(pFile);
}

void LexerStage::DoStage(std::wistream& inputStream, std::wostream& outputStream)
{
	ReadText(inputStream);
	ParseText();
	SaveTokens(outputStream);
}

void LexerStage::ReadText(std::wistream& inputStream)
{
	m_inputText = std::wstring(std::istreambuf_iterator<wchar_t>(inputStream), std::istreambuf_iterator<wchar_t>());
}

void LexerStage::ParseText()
{
	size_t currCharIdx = 0;

	size_t currentLine = 0;

	std::wstring::iterator currCharacter = m_inputText.begin() + currCharIdx;

	while (currCharacter != m_inputText.end())
	{
		currCharacter = m_inputText.begin() + currCharIdx;

		bool foundCorrectType = false;

		std::wsmatch resultedmatch;

		for (size_t i = 0; i < m_tokenRules.size(); i++)
		{
			if (std::regex_search(std::wstring::const_iterator(currCharacter), m_inputText.cend(), resultedmatch, m_tokenRules[i].Regexpr)
				&& resultedmatch.position(0) == 0)
			{
				m_tokens.emplace_back(resultedmatch.str(0), m_tokenRules[i].Type);
				foundCorrectType = true;

				size_t len = resultedmatch.length(0);

				currCharIdx += len;

				std::wstring resultedString = resultedmatch.str(0);

				for (auto& iter : resultedString) if (iter == L'\n') currentLine++;

				break;
			}
		}

		ASSERT2(foundCorrectType, std::wstring(L"Cannot recognize token at line: ") + std::to_wstring(currentLine));
	}
}

void LexerStage::SaveTokens(std::wostream& outputStream)
{
	typedef rapidjson::GenericDocument<rapidjson::UTF16<>> WDocument;
	typedef rapidjson::GenericValue<rapidjson::UTF16<>> WValue;
	typedef rapidjson::GenericStringBuffer<rapidjson::UTF16<>> WStringBuffer;

	WDocument doc;
	doc.SetObject();

	WValue myArray(rapidjson::kArrayType);
	WDocument::AllocatorType& allocator = doc.GetAllocator();

	for (size_t i = 0; i < m_tokens.size(); i++)
	{
		WValue objValue;
		objValue.SetObject();

		WValue tmp;
		tmp.SetString(m_tokens[i].Type.c_str(), m_tokens[i].Type.length());
		objValue.AddMember(L"Type", tmp, allocator);

		tmp = WValue();
		tmp.SetString(m_tokens[i].Value.c_str(), m_tokens[i].Value.length());
		objValue.AddMember(L"Value", tmp, allocator);

		myArray.PushBack(objValue, allocator);
	}

	doc.AddMember(L"Tokens", myArray, allocator);
	WStringBuffer strbuf;
	rapidjson::Writer<WStringBuffer, rapidjson::UTF16<>> writer(strbuf);
	doc.Accept(writer);

	std::wstring result = strbuf.GetString();

	outputStream.write(result.c_str(), result.length());
}

void LexerStage::InitRules(rapidjson::Document& doc)
{
	auto rules = doc["Rules"].GetArray();

	for (auto i = rules.Begin(); i < rules.End(); i++)
	{
		TokenRule newRule;

		newRule.Regexpr = Utils::StringConverter::StrToWstr((*i)["Regexp"].GetString());
		newRule.Type	= Utils::StringConverter::StrToWstr((*i)["Type"].GetString());
		m_tokenRules.emplace_back(newRule);
	}
}
