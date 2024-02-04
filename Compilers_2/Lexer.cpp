#include "Lexer.h"

Lexer::Lexer() {
    inputFile.open("Test_1.txt", std::ios::in);

    if (!inputFile.is_open()) {
        std::cerr << "File acess denied!" << std::endl;
    }
    numberInRow = 1;
    numberInColumn = 1;
}

Lexer::Lexer(const std::string& filename) {
    inputFile.open(filename, std::ios::in);

    if (!inputFile.is_open()) {
        std::cerr << "File acess denied!" << std::endl;
    }
    numberInRow = 1;
    numberInColumn = 1;
}

Lexer::~Lexer() {
    inputFile.close();
}

void Lexer::Start() {
    State curState = BASE;
    char c;
    inputFile.get(c);
    bool stopFlag = false;
    while (!inputFile.eof() && !stopFlag) {
        switch (curState)
        {
        case BASE: {
            while ((c == ' ') || (c == '\t') || (c == '\n') || (c == '\0'))
            {
                if (c == '\n') {
                    numberInColumn++;
                    numberInRow = 0;
                }
                inputFile.get(c);
                numberInRow++;

                if (inputFile.eof()) {
                    break;
                }
            }

            if (inputFile.eof()) {
                stopFlag = true;
                break;
            }

            if (matchPatternWithChar(c, Patterns[0])) { // ID, KWORD
                // std::cout << "0";
                curState = ID;
            }
            else if (matchPatternWithChar(c, Patterns[4])) { // DELIM
                // std::cout << "1";
                curState = DLM;
            }
            else if (matchPatternWithChar(c, Patterns[2])) { // NUM
                // std::cout << "2";
                curState = NUM;
            }
            else if (matchPatternWithChar(c, Patterns[6])) { // DSIGN
                // std::cout << "3";
                curState = DSIGN;
            }
            else if (matchPatternWithChar(c, Patterns[3])) { // LIT
                // std::cout << "4";
                curState = LIT;
            }
            else {
                std::cout << "This not founded: " << c;
                std::cin >> c;
            }

            break;
        }

        case ID: {
            std::string buff;
            Lexeme lex;
            unsigned short int saveNInRow = numberInRow;

            while (matchPatternWithChar(c, Patterns[1])) {
                buff += c;
                inputFile.get(c);
                numberInRow++;

                if (inputFile.eof()) {
                    break;
                }
            }

            if (isKeyWord(buff)) {
                // std::cout << buff << ' ';
                lex = { checkLexemClass(buff), 5, numberInColumn, saveNInRow, 0 }; // kword, colNum, rowNum, inTableNum
                //std::cout << numberInColumn << " " << saveNInRow << std::endl;
            }
            else {
                lex = { checkLexemClass("IDENTIF"), 3, numberInColumn, saveNInRow, checkReprId(buff) }; // kword, colNum, rowNum, inTableNum
                //std::cout << numberInColumn << " " << saveNInRow << std::endl;
                //std::cout << "Id: " << tmp << std::endl;
            }

            lexStream.push_back(lex);
            curState = BASE;
            break;
        }

        case NUM: {
            std::string buff;
            Lexeme lex;
            unsigned short int saveNInRow = numberInRow;

            while (matchPatternWithChar(c, Patterns[2])) {
                buff += c;
                inputFile.get(c);
                numberInRow++;

                if (inputFile.eof()) {
                    c = ' ';
                    break;
                }
            }

            if ((!matchPatternWithChar(c, Patterns[4]) && !matchPatternWithChar(c, Patterns[5]))
                || (buff[0] == '0' && (buff.size() > 1))) {
                curState = ERROR;
                std::cerr << "StateMachine error at " << buff << " lexem! Uncorrect number" << std::endl;
                break;
            }

            lex = { checkLexemClass("CONST"), 4, numberInColumn, saveNInRow, checkConst(buff) };
            lexStream.push_back(lex);
            curState = BASE;
            break;
        }

        case DLM: {
            Lexeme lex;
            lex = { checkLexemClass(std::string(1, c)), 2, numberInColumn, numberInRow, 0 };
            lexStream.push_back(lex);
            inputFile.get(c);
            numberInRow++;
            curState = BASE;
            break;
        }
        case LIT: {
            std::string buff;
            Lexeme lex;
            lex = { checkLexemClass(std::string(1, c)), 2, numberInColumn, numberInRow, 0 };
            lexStream.push_back(lex);
            inputFile.get(c);
            numberInRow++;

            unsigned short int saveNInRow = numberInRow;

            while (c != '\"' && c != '\n') {
                buff += c;
                inputFile.get(c);
                numberInRow++;

                if (inputFile.eof()) {
                    break;
                }
            }
            lex = { checkLexemClass("CONST"), 4, numberInColumn, saveNInRow, checkConst(buff) };
            lexStream.push_back(lex);

            if (c == '\"') {
                lex = { checkLexemClass(std::string(1, c)), 2, numberInColumn, numberInRow, 0 };
                lexStream.push_back(lex);
                inputFile.get(c);
                numberInRow++;
            }
            else if (c == '\n') {
                std::cerr << "StateMachine error at " << buff << ". Uncorrect literal const " << std::endl;
                curState = ERROR;
                break;
            }

            curState = BASE;
            break;
        }
        case DSIGN: {
            Lexeme lex;
            char c_prev = c;
            inputFile.get(c);

            if (c_prev == '/' && c == '/') {
                while (c != '\n') inputFile.get(c);
                curState = BASE;
                break;
            }
            else if (c_prev == '/' && c == '*') {
                while (!(c_prev == '*' && c == '/')) {
                    c_prev = c;
                    inputFile.get(c);
                }
                inputFile.get(c);
                curState = BASE;
                break;
            }
            else if (c_prev == '/') {
                lex = { checkLexemClass(std::string(1, c)), 2, numberInColumn, numberInRow, 0 };
                numberInRow++;
                lexStream.push_back(lex);
                curState = BASE;
                break;
            }

            if (c != '=' && c_prev != '!') {
                lex = { checkLexemClass(std::string(1, c_prev)), 2, numberInColumn, numberInRow, 0 };
                numberInRow++;
                lexStream.push_back(lex);
                curState = BASE;
                break;
            }
            else if (c != '=' && c_prev == '!') {
                std::cerr << "StateMachine error at " << c_prev << " lexem! Uncorrect symbol" << std::endl;
                curState = ERROR;
                break;
            }

            if (c_prev == '=') {
                lex = { checkLexemClass("=="), 2, numberInColumn, numberInRow, 0 };
            }
            else if (c_prev == '>') {
                lex = { checkLexemClass(">="), 2, numberInColumn, numberInRow, 0 };
            }
            else if (c_prev == '<') {
                lex = { checkLexemClass("<="), 2, numberInColumn, numberInRow, 0 };
            }
            else if (c_prev == '!') {
                lex = { checkLexemClass("!="), 2, numberInColumn, numberInRow, 0 };
            }
            inputFile.get(c);
            numberInRow += 2;

            lexStream.push_back(lex);
            curState = BASE;
            break;
        }
        case ERROR: {
            std::cerr << "Ñritical error - programm stopped" << std::endl;
            stopFlag = true;
            break;
        }
        default:
            std::cerr << "StateMachine defautl error!" << std::endl;
            break;
        }
    }
}

void Lexer::DisplayTables() {
    std::cout << '#' << "\t" << std::setw(15) << std::left << "Lexem Class" << "\t" << "Lexem Type" << std::endl;
    for (int i = 0; i < lexStream.size(); i++) {
        /*
        if (lexemClass[lexStream[i].lexClass] == "IDENTIF") {
            std::cout << i << "\t" << std::setw(15) << std::left << reprIdTable[lexStream[i].numInTable]
                << "\t" << lexemType[lexStream[i].lexType] << std::endl;
        }
        else if (lexemClass[lexStream[i].lexClass] == "CONST") {
            std::cout << i << "\t" << std::setw(15) << std::left << constTable[lexStream[i].numInTable]
                << "\t" << lexemType[lexStream[i].lexType] << std::endl;
        }
        else {
            std::cout << i << "\t" << std::setw(15) << std::left << lexemClass[lexStream[i].lexClass]
                << "\t" << lexemType[lexStream[i].lexType] << std::endl;
        }
        */
        
        std::cout << i << "\t" << std::setw(15) << std::left << lexemClass[lexStream[i].lexClass]
            << "\t" << lexemType[lexStream[i].lexType] << std::endl;
    }

    std::cout << '\n' << '#' << "\t" << "Name of Identifier" << std::endl;
    for (int i = 0; i < reprIdTable.size(); i++) {
        std::cout << i << "\t" << reprIdTable[i] << std::endl;
    }

    std::cout << '\n' << '#' << "\t" << "Constant" << std::endl;
    for (int i = 0; i < constTable.size(); i++) {
        std::cout << i << "\t" << constTable[i] << std::endl;
    }

    std::cout << '\n';
}

unsigned short int Lexer::checkReprId(const std::string& buff) {
    for (unsigned short int i = 0; i < reprIdTable.size(); i++) {
        if (reprIdTable[i] == buff) {
            return i;
        }
    }
    reprIdTable.push_back(buff);
    return reprIdTable.size() - 1;
}

unsigned short int Lexer::checkLexemClass(const std::string& buff) {
    for (unsigned short int i = 0; i < lexemClass.size(); i++) {
        if (lexemClass[i] == buff) {
            return i;
        }
    }
    lexemClass.push_back(buff);
    return lexemClass.size() - 1;
}

unsigned short int Lexer::checkConst(const std::string& buff) {
    for (unsigned short int i = 0; i < constTable.size(); i++) {
        if (constTable[i] == buff) {
            return i;
        }
    }
    constTable.push_back(buff);
    return constTable.size() - 1;
}