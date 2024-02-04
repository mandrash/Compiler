#pragma once
#include <fstream>
#include <iomanip>
#include "Addons.h"

class Lexer
{
private:
    std::ifstream inputFile;
    std::vector<Lexeme> lexStream;
    unsigned short int numberInRow;
    unsigned short int numberInColumn;
    std::vector<std::string> reprIdTable;
    std::vector<std::string> constTable;
    std::vector<std::string> lexemClass;

public:
    Lexer(const std::string& filename);
    Lexer();
    ~Lexer();

    void Start();
    void DisplayTables();
    std::vector<Lexeme> GetLexStream() { return std::move(lexStream); }
    std::vector<std::string> GetReprIdTable() { return std::move(reprIdTable); }
    std::vector<std::string> GetConstTable() { return std::move(constTable); }
    std::vector<std::string> GetLexemClasses() { return std::move(lexemClass); }
    unsigned short int checkReprId(const std::string& buff); // проверка и добавление в таблицу представлений Id
    unsigned short int checkLexemClass(const std::string& buff); // проверка таблица представления лексем
    unsigned short int checkConst(const std::string& buff);// проверка таблицы констант
};