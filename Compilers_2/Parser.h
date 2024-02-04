#pragma once
#include "Addons.h"
#include <iomanip>

class Parser
{
private:
	// Поля получаемые от класса Lexer
	std::vector<Lexeme> lexStream;
	std::vector<std::string> reprIdTable;
	std::vector<std::string> constTable;
	std::vector<std::string> lexemClass;

	// Собственные поля
	size_t lexIndex;
	std::shared_ptr<Node> syntaxTree;
	std::vector<std::string> idType;
	std::vector<Id> idTable;

public:

	Parser(std::vector<Lexeme> lexStream) : lexStream(lexStream), lexIndex(0), syntaxTree(nullptr) {}
	Parser(std::vector<Lexeme> lexStream, std::vector<std::string> reprIdTable, 
		std::vector<std::string> constTable, std::vector<std::string> lexemClass) 
		: lexStream(lexStream), lexIndex(0), syntaxTree(nullptr), reprIdTable(reprIdTable), 
		constTable(constTable), lexemClass(lexemClass) {}

	void Parse(); // Метод, вызывающий начальное правило (присвоение корневого указателя дереву)
	std::vector<std::string> GetReprIdTable() { return std::move(reprIdTable); }
	std::vector<std::string> GetConstTable() { return std::move(constTable); }
	std::vector<std::string> GetIdType() { return std::move(idType); }
	std::shared_ptr<Node> MoveSyntaxTree() { return std::move(syntaxTree); }
	std::string CurrentLexClass(); // Возвращает класс текущей лексемы
	std::shared_ptr<Node> GetSyntaxTree() { return syntaxTree; }
	std::shared_ptr<Node> ParseS(); // Начальное правило
	std::shared_ptr<Node> ParseBody();
	void ParseOperator(std::vector<std::shared_ptr<Node>>& children);
	std::shared_ptr<Node> ParseDeclaration();
	std::shared_ptr<Node> ParseAssignment();
	std::shared_ptr<Node> ParseExpression();
	std::shared_ptr<Node> ParseLConst();
	std::shared_ptr<Node> ParseMult();
	std::shared_ptr<Node> ParseGroup();
	std::shared_ptr<Node> ParseCallF();
	std::shared_ptr<Node> ParseOperation();
	std::shared_ptr<Node> ParseConst();
	std::shared_ptr<Node> ParseIdentif();
	std::shared_ptr<Node> ParseConditionOperator();
	std::shared_ptr<Node> ParseCondition();
	std::shared_ptr<Node> ParseLoop();
	std::shared_ptr<Node> ParseReturn();
	std::shared_ptr<Node> ParseFunction();
	std::shared_ptr<Node> ParseArguments(); 
	std::shared_ptr<Node> Match(const std::string& expectedLex);
	std::shared_ptr<Node> Match(const std::vector<std::string>& expectedLexemes);
	bool IsMatch(const std::vector<std::string>& expectedLexemes);
	bool IsMatch(const std::string& expectedLex);
	void NextLexeme();
	void PrintIdTypeTable();
	void PrintIdTable();
};

