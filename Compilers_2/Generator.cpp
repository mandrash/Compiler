#include "Generator.h"

void Generator::Start()
{
    std::ofstream outputFile("output.asm");
    
    data = "%include \"io.inc\"\nsection .data\n";
    code = "section .text\n    global main\nmain:\n";

    generateBodyCode(syntaxTree);

    outputFile << data;
    outputFile << code;

    outputFile.close();

    std::cout << "\nAssembly code generated successfully.\n\n";
}

void Generator::generateBodyCode(const std::shared_ptr<Node>& node) {
    if (!node) {
        return;
    }

    // Генерация кода для объявления переменой
    if (node->value == "<Declaration>") {
        if (node->children[0]->value == "string") {
            data += "    " + node->children[1]->children[0]->value + " dd \"\", 0\n";
        } 
        else {
            data += "    " + node->children[1]->children[0]->value + " dd 0\n";
        }
        return;
    }
    // Генерация кода для присваивания значения переменой
    else if (node->value == "<Assignment>") {
        std::string variableName = node->children[0]->children[0]->value;
        std::shared_ptr<Node> expressionNode = node->children[2];

        code += "    ; Assignment to " + variableName + "\n";

        generateExpressionCode(expressionNode);

        code += "    mov ebx, " + variableName + " ; Get the address of the variable\n";

        code += "    mov [ebx], eax ; Store the result in the variable\n";

        return;
    }
    // Генерация кода для условного оператора
    else if (node->value == "<ConditionOper>") {
        code += "    ; Condition Operator\n";

        bool elseFlag = node->children.size() > 6;

        generateConditionCode(node->children[2]);

        code += elseFlag ? "else" : "endif";
        code += std::to_string(ifId) + "\n";

        generateBodyCode(node->children[5]);

        if (elseFlag) {
            code += "    jmp endif" + std::to_string(ifId) + "\n";
            code += "else" + std::to_string(ifId) + ":\n";
            generateBodyCode(node->children[7]);
        }
        
        code += "endif" + std::to_string(ifId) + ":\n";

        ifId++;

        return;
    }
    // Генерация кода для цикла
    else if (node->value == "<Loop>") {
        code += "    ; Loop\n";

        code += "while" + std::to_string(whileId) + ":\n";

        generateConditionCode(node->children[1]);

        code += "endwhile" + std::to_string(whileId) + "\n";

        generateBodyCode(node->children[3]);

        code += "    jmp while" + std::to_string(whileId) + "\n";
        code += "endwhile" + std::to_string(whileId) + ":\n";

        whileId++;

        return;
    }
    // Генерация кода для функции
    else if (node->value == "<Function>") {
        code += "    xor eax, eax\n    ret\n\n";
        code += node->children[2]->children[0]->value + ":\n";

        generateBodyCode(node->children[7]);

        code += "    ret\n";

        return;
    }

    for (const auto& child : node->children) {
        generateBodyCode(child);
    }
}

void Generator::generateExpressionCode(const std::shared_ptr<Node>& node)
{
    // Генерация кода выражений
    if (node->children[0]->value == "<CONST>" || node->children[0]->value == "<IDENTIF>" || node->children[0]->value == "<CallFunction>") {
        std::string operand = node->children[0]->children[0]->value;

        if (operand == "false") operand = "0";
        if (operand == "true") operand = "1";

        if (std::isdigit(operand[0])) {
            // Операнд - число
            code += "    mov eax, " + operand + " ; Load constant " + operand + " to eax\n";
        }
        else if (operand[0] == '\"') {
            // Операнд - строка
            std::string str = node->children[0]->children[1]->value;
            code += "    mov eax, \"" + str + "\" ; String content\n";
        }
        else if (node->children[0]->value == "<CallFunction>") {
            code += "    call " + getOperand(node) + "\n";
            code += "    mov eax, [result] ; Load variable result to eax\n"; 
        }
        else {
            // Операнд - переменная
            code += "    mov eax, [" + operand + "] ; Load variable " + operand + " to eax\n";
        }
        return;
    }

    std::shared_ptr<Node> curNode = node->children[0];

    generateOperandCode(curNode->children[0]);

    code += "    mov eax, edx\n";

    for (int i = 2; i < curNode->children.size(); i += 2) {
        
        generateOperandCode(curNode->children[i]);

        if (curNode->children[i - 1]->value == "+") {
            code += "    add eax, edx ; Add the result to eax\n";
        }
        else if (curNode->children[i - 1]->value == "-") {
            code += "    sub eax, edx ; Subtract the result from eax\n";
        }
        //code += curNode->children[i]->value + '\n';
    }
    
}

void Generator::generateOperandCode(const std::shared_ptr<Node>& node)
{
    // Генерация кода операнды
    std::string operand = getOperand(node);

    if (operand == "false") operand = "0";
    if (operand == "true") operand = "1";

    if (std::isdigit(operand[0])) {
        // Операнд - число
        code += "    mov edx, " + operand + " ; Load constant " + operand + " to edx\n";
    }
    else if (operand[0] == '\"') {
        // Операнд - строка
        std::string str = node->children[0]->children[1]->value;
        code += "    mov edx, \"" + str + "\" ; String content\n";
    }
    else if (node->children[0]->children[0]->value == "<CallFunction>") {
        code += "    call " + getOperand(node->children[0]) + "\n";
        code += "    mov edx, [result] ; Load variable result to eax\n";
    }
    else {
        // Операнд - переменная
        code += "    mov edx, [" + operand + "] ; Load variable " + operand + " to edx\n";
    }
    return;
}

void Generator::generateConditionCode(const std::shared_ptr<Node>& node)
{
    // Генерация кода условия
    std::string operation = node->children[1]->value;

    generateExpressionCode(node->children[0]);

    code += "    mov ebx, eax\n";

    generateExpressionCode(node->children[2]);

    code += "    cmp ebx, eax\n";

    if (operation == "==") {
        code += "    jne ";
    }
    else if (operation == "!=") {
        code += "    jn ";
    }
    else if (operation == ">") {
        code += "    jle ";
    }
    else if (operation == "<") {
        code += "    jge ";
    }
    else if (operation == ">=") {
        code += "    jl ";
    }
    else if (operation == "<=") {
        code += "    jg ";
    }

    return;
}

std::string Generator::getOperand(const std::shared_ptr<Node>& node)
{

    return node->children[0]->children[0]->children[0]->value;
}

void Generator::printAsmCode()
{
    std::cout << data;
    std::cout << code;
}
