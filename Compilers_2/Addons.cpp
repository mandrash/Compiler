#include "Addons.h"

std::map<unsigned short int, std::string> lexemType = {
    {1, "LITERAL"},  {2, "DELIM"},  {3, "IDENT"},  {4, "CONST"},  {5, "KWORD"}
};

std::vector<std::string> keyWords = {
    "main","integer","string","bool",
    "if", "then", "else", "endif",
    "while", "endwhile", "true", "false",
    "return", "proc"
};

std::vector<std::string> Patterns = {
    "[A-za-z_]","[\\w_]","[\\d]","[\"]","[;,(){}\\*\\+-]","[\\s]","[/=><!]"
};

void printTree(const std::shared_ptr<Node>& node, int level) {
    for (int i = 0; i < level; ++i) {
        std::cout << "  ";
    }
    std::cout << "|- " << node->value << std::endl;

    for (size_t i = 0; i < node->children.size(); ++i) {
        if (i == node->children.size() - 1) {
            std::cout << std::string(2 * (level + 1), ' ') << "\\-";
        }
        else {
            std::cout << std::string(2 * (level + 1), ' ') << "|-";
        }
        printTree(node->children[i], level + 1);
    }
}

bool matchPatternWithChar(char c, std::string pattern) {
    std::string input = std::string(1, c);
    std::regex regPattern(pattern);
    return std::regex_match(input, regPattern);
}

bool isKeyWord(const std::string& obj) {

    return std::find(keyWords.begin(), keyWords.end(), obj) != keyWords.end();
}