#include "Lexer.h"
#include "Parser.h"
#include "Generator.h"

using namespace std;

int main() {
    Lexer lex("test.txt");
    lex.Start();
    //lex.DisplayTables();

    Parser parser(lex.GetLexStream(), lex.GetReprIdTable(), lex.GetConstTable(), lex.GetLexemClasses());
    parser.Parse();
    printTree(parser.GetSyntaxTree());
    //parser.PrintIdTypeTable();

    Generator gener(parser.GetReprIdTable(), parser.GetConstTable(), parser.GetIdType(), parser.MoveSyntaxTree());
    gener.Start();
    gener.printAsmCode();

    return 0;
}