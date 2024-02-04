#pragma once
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <regex>

extern std::map<unsigned short int, std::string> lexemType;

extern std::vector<std::string> keyWords;

extern std::vector<std::string> Patterns;

enum State {
    BASE,
    ERROR,
    ID,
    NUM,
    DLM,
    LIT,
    DSIGN,
};

// Структура Лексемы
typedef struct  Lexeme
{
    unsigned short int lexClass;
    unsigned short int lexType; // общий тип, KWORD, ID....
    unsigned short int n_col;
    unsigned short int n_row;
    unsigned short int numInTable;

    Lexeme() : lexClass(0), lexType(0), n_row(0), n_col(0), numInTable(0) {}
    Lexeme(unsigned short int lCl, unsigned short int lType, unsigned short int n_r, unsigned short int n_c, unsigned short int numIn)
        : lexClass(lCl), lexType(lType), n_row(n_r), n_col(n_c), numInTable(numIn) {}
}Lexeme;

struct Id {
    std::string name;
    std::string type;
    std::string value;

    Id() {}
    Id(std::string name, std::string type, std::string value) : name(name), type(type), value(value) {}
};

// Узел дерева (используется в синтаксическом анализаторе), можно добавить поле для хранения лексемы
class Node {
public:
    std::string value;
    std::vector<std::shared_ptr<Node>> children;

    Node(const std::string& value) : value(value) {}
    Node(const std::string& value, const std::vector<std::shared_ptr<Node>>& children) : value(value), children(children) {}
};

// Вспомогательные функции
void printTree(const std::shared_ptr<Node>& node, int level = 0);
bool matchPatternWithChar(char c, std::string pattern);
bool isKeyWord(const std::string& obj);