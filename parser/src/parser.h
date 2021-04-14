//
// Created by ccjeff on 2021/4/7.
//
#ifndef SIMPLE_COMPILER_FOR_MIPS_PARSER_H
#define SIMPLE_COMPILER_FOR_MIPS_PARSER_H


#include <cassert>
#include <cstdlib>
#include <exception>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "scanner.h"

using std::string;
using std::shared_ptr;
using std::make_shared;

// throw out error message in parser caused by unexpected termination
class UnexpectedTerminationException : public std::exception {
private:
    string expectTokenValue;
    string formattedMessage;

public:
    UnexpectedTerminationException()
            : formattedMessage("We got unexpected EOF") {
    }

    explicit UnexpectedTerminationException(string expectTokenValue)
            : expectTokenValue(std::move(expectTokenValue)) {
        std::stringstream buffer;
        buffer << "We need '" << this->expectTokenValue << "', but got nothing";
        formattedMessage = buffer.str();
    }

    const char *what() const noexcept override {
        return formattedMessage.c_str();
    }
};

//Throw out exception for unexpected token. Means a syntax error.
class UnexpectedTokenException : public std::exception {
private:
    string actualTokenValue, expectTokenValue;
    string formattedMessage;
public:
    explicit UnexpectedTokenException(string actualTokenValue)
            : actualTokenValue(std::move(actualTokenValue)) {
        std::stringstream buffer;
        buffer << "We got an unwanted '" << this->actualTokenValue << "'";
        formattedMessage = buffer.str();
    }

    UnexpectedTokenException(string actualTokenValue, string expectTokenValue)
            : actualTokenValue(std::move(actualTokenValue)), expectTokenValue(std::move(expectTokenValue)) {
        std::stringstream buffer;
        buffer << "We got an unwanted '" << this->actualTokenValue
               << "', but we need '" << this->expectTokenValue << "'";
        formattedMessage = buffer.str();
    }

    const char *what() const noexcept override {
        return formattedMessage.c_str();
    }
};

// the general syntax node for the AST. Inherited by different nodes.
class AbstractSyntaxNode {
public:
    static void validateTokenType(Scanner &scanner, TokenType tokenType) {
        if (!scanner.hasToken()) {
            throw UnexpectedTokenException(token2string(tokenType));
        }
        if (scanner.getTokenType() != tokenType) {
            throw UnexpectedTokenException(token2string(tokenType), scanner.getTokenValue());
        }
    }

    virtual void fillPrettyStrings(std::vector<string> &rows, unsigned indentBase, unsigned indentFactor) = 0;
};

class Program;

class VarDeclarations;

class VarDeclaration;

class DeclarationList;

class Declaration;

class CodeBlock;

class Statements;

class Statement;

class ControlStatement;

class ReadWriteStatement;

class AssignStatement;

class IfStatement;

class IfStmt;

class WhileStatement;

class DoWhileStatement;

class ReturnStatement;

class ReadStatement;

class WriteStatement;

class Exp;

class Exp2;

class Exp3;

class Exp4;

class Exp5;

class Exp6;

class Exp7;

class Exp8;

class Exp9;

class Exp10;

class Exp11;

class Program : public AbstractSyntaxNode {
private:
    shared_ptr<VarDeclarations> nodeVarDeclarations;
    shared_ptr<Statements> nodeStatements;

public:
    Program(const shared_ptr<VarDeclarations> &nodeVarDeclarations,
            const shared_ptr<Statements> &nodeStatements) {
        assert(nodeVarDeclarations && nodeStatements);
        this->nodeVarDeclarations = nodeVarDeclarations;
        this->nodeStatements = nodeStatements;
    }

    // program : var_declarations statements
    static shared_ptr<Program> parse(Scanner &scanner);

    void fillPrettyStrings(std::vector<string> &rows, unsigned indentBase, unsigned indentFactor) override;
};

class VarDeclarations : public AbstractSyntaxNode {
private:
    shared_ptr<VarDeclaration> nodeVarDeclaration;
    shared_ptr<VarDeclarations> nodeVarDeclarations;

public:
    VarDeclarations(const shared_ptr<VarDeclaration> &nodeVarDeclaration,
                    const shared_ptr<VarDeclarations> &nodeVarDeclarations)
            : nodeVarDeclaration(nodeVarDeclaration),
              nodeVarDeclarations(nodeVarDeclarations) {
        assert(!nodeVarDeclarations || nodeVarDeclaration);
    }

    // var_declarations : var_declaration var_declarations | ε
    static shared_ptr<VarDeclarations> parse(Scanner &scanner);

    void fillPrettyStrings(std::vector<string> &rows, unsigned indentBase, unsigned indentFactor) override;
};

class VarDeclaration : public AbstractSyntaxNode {
private:
    shared_ptr<DeclarationList> nodeDeclarationList;

public:
    explicit VarDeclaration(const shared_ptr<DeclarationList> &nodeDeclarationList)
            : nodeDeclarationList(nodeDeclarationList) {
        assert(nodeDeclarationList);
    }

    // var_declaration : INT declaration_list SEMI
    static shared_ptr<VarDeclaration> parse(Scanner &scanner);

    void fillPrettyStrings(std::vector<string> &rows, unsigned indentBase, unsigned indentFactor) override;
};

class DeclarationList : public AbstractSyntaxNode {
private:
    shared_ptr<Declaration> nodeDeclaration;
    shared_ptr<DeclarationList> nodeDeclarationList;

public:
    DeclarationList(const shared_ptr<Declaration> &nodeDeclaration,
                    const shared_ptr<DeclarationList> &nodeDeclarationList)
            : nodeDeclaration(nodeDeclaration),
              nodeDeclarationList(nodeDeclarationList) {
        assert(nodeDeclaration);
    }

    // declaration_list : declaration | declaration COMMA declaration_list
    static shared_ptr<DeclarationList> parse(Scanner &scanner);

    void fillPrettyStrings(std::vector<string> &rows, unsigned indentBase, unsigned indentFactor) override;
};

class Declaration : public AbstractSyntaxNode {
private:
    string valueId;
    int valueNumber;
    bool hasNumber;
    bool isArray;

public:
    explicit Declaration(string valueId)
            : valueId(std::move(valueId)), valueNumber(0), hasNumber(false), isArray(false) {
        assert(!this->valueId.empty());
    }

    Declaration(string valueId, int valueNumber, bool isArray)
            : valueId(std::move(valueId)), valueNumber(valueNumber), hasNumber(true), isArray(isArray) {
        assert(!this->valueId.empty());
    }

    // declaration : IDENTIFIER ASSIGN NUMBER | IDENTIFIER LSQUARE NUMBER RSQUARE | IDENTIFIER
    static shared_ptr<Declaration> parse(Scanner &scanner);

    void fillPrettyStrings(std::vector<string> &rows, unsigned indentBase, unsigned indentFactor) override;
};

class CodeBlock : public AbstractSyntaxNode {
private:
    shared_ptr<Statement> nodeStatement;
    shared_ptr<Statements> nodeStatements;

public:
    explicit CodeBlock(const shared_ptr<Statement> &nodeStatement)
            : nodeStatement(nodeStatement), nodeStatements(nullptr) {
        assert(nodeStatement);
    }

    explicit CodeBlock(const shared_ptr<Statements> &nodeStatements)
            : nodeStatement(nullptr), nodeStatements(nodeStatements) {
        assert(nodeStatements);
    }

    // code_block : statement | LBRACE statements RBRACE
    static shared_ptr<CodeBlock> parse(Scanner &scanner);

    void fillPrettyStrings(std::vector<string> &rows, unsigned indentBase, unsigned indentFactor) override;
};

class Statements : public AbstractSyntaxNode {
private:
    shared_ptr<Statement> nodeStatement;
    shared_ptr<Statements> nodeStatements;

public:
    explicit Statements(const shared_ptr<Statement> &nodeStatement,
                        const shared_ptr<Statements> &nodeStatements)
            : nodeStatement(nodeStatement), nodeStatements(nodeStatements) {
        assert(nodeStatement);
    }

    // statements : statement | statement statements
    static shared_ptr<Statements> parse(Scanner &scanner);

    void fillPrettyStrings(std::vector<string> &rows, unsigned indentBase, unsigned indentFactor) override;
};

class Statement : public AbstractSyntaxNode {
private:
    shared_ptr<AssignStatement> nodeAssignStatement;
    shared_ptr<ControlStatement> nodeControlStatement;
    shared_ptr<ReadWriteStatement> nodeReadWriteStatement;

public:
    // statement => SEMI
    Statement() : nodeAssignStatement(nullptr),
                  nodeControlStatement(nullptr),
                  nodeReadWriteStatement(nullptr) {
    }

    // statement => assign_statement SEMI
    explicit Statement(const shared_ptr<AssignStatement> &nodeAssignStatement)
            : nodeAssignStatement(nodeAssignStatement),
              nodeControlStatement(nullptr),
              nodeReadWriteStatement(nullptr) {
        assert(nodeAssignStatement);
    }

    // statement => control_statement
    explicit Statement(const shared_ptr<ControlStatement> &nodeControlStatement)
            : nodeAssignStatement(nullptr),
              nodeControlStatement(nodeControlStatement),
              nodeReadWriteStatement(nullptr) {
        assert(nodeControlStatement);
    }

    // statement => read_write_statement SEMI
    explicit Statement(const shared_ptr<ReadWriteStatement> &nodeReadWriteStatement)
            : nodeAssignStatement(nullptr),
              nodeControlStatement(nullptr),
              nodeReadWriteStatement(nodeReadWriteStatement) {
        assert(nodeReadWriteStatement);
    }

    // statement : assign_statement SEMI | control_statement | read_write_statement SEMI | SEMI
    static shared_ptr<Statement> parse(Scanner &scanner);

    void fillPrettyStrings(std::vector<string> &rows, unsigned indentBase, unsigned indentFactor) override;
};

class ControlStatement : public AbstractSyntaxNode {
private:
    shared_ptr<IfStatement> nodeIfStatement;
    shared_ptr<WhileStatement> nodeWhileStatement;
    shared_ptr<DoWhileStatement> nodeDoWhileStatement;
    shared_ptr<ReturnStatement> nodeReturnStatement;

public:
    explicit ControlStatement(const shared_ptr<IfStatement> &nodeIfStatement)
            : nodeIfStatement(nodeIfStatement),
              nodeWhileStatement(nullptr),
              nodeDoWhileStatement(nullptr),
              nodeReturnStatement(nullptr) {
        assert(nodeIfStatement);
    }

    explicit ControlStatement(const shared_ptr<WhileStatement> &nodeWhileStatement)
            : nodeIfStatement(nullptr),
              nodeWhileStatement(nodeWhileStatement),
              nodeDoWhileStatement(nullptr),
              nodeReturnStatement(nullptr) {
        assert(nodeWhileStatement);
    }

    explicit ControlStatement(const shared_ptr<DoWhileStatement> &nodeDoWhileStatement)
            : nodeIfStatement(nullptr),
              nodeWhileStatement(nullptr),
              nodeDoWhileStatement(nodeDoWhileStatement),
              nodeReturnStatement(nullptr) {
        assert(nodeDoWhileStatement);
    }

    explicit ControlStatement(const shared_ptr<ReturnStatement> &nodeReturnStatement)
            : nodeIfStatement(nullptr),
              nodeWhileStatement(nullptr),
              nodeDoWhileStatement(nullptr),
              nodeReturnStatement(nodeReturnStatement) {
        assert(nodeReturnStatement);
    }

    // control_statement : if_statement | while_statement | do_while_statement SEMI | return_statement SEMI
    static shared_ptr<ControlStatement> parse(Scanner &scanner);

    void fillPrettyStrings(std::vector<string> &rows, unsigned indentBase, unsigned indentFactor) override;
};

class ReadWriteStatement : public AbstractSyntaxNode {
private:
    shared_ptr<ReadStatement> nodeReadStatement;
    shared_ptr<WriteStatement> nodeWriteStatement;

public:
    explicit ReadWriteStatement(const shared_ptr<ReadStatement> &nodeReadStatement)
            : nodeReadStatement(nodeReadStatement), nodeWriteStatement(nullptr) {
        assert(nodeReadStatement);
    }

    explicit ReadWriteStatement(const shared_ptr<WriteStatement> &nodeWriteStatement)
            : nodeReadStatement(nullptr), nodeWriteStatement(nodeWriteStatement) {
        assert(nodeWriteStatement);
    }

    // read_write_statement : read_statement | write_statement
    static shared_ptr<ReadWriteStatement> parse(Scanner &scanner);

    void fillPrettyStrings(std::vector<string> &rows, unsigned indentBase, unsigned indentFactor) override;
};

class AssignStatement : public AbstractSyntaxNode {
private:
    string valueId;
    shared_ptr<Exp> nodeExpIndex;
    shared_ptr<Exp> nodeExp;

public:
    AssignStatement(string valueId,
                    const shared_ptr<Exp> &nodeExpIndex,
                    const shared_ptr<Exp> &nodeExp)
            : valueId(std::move(valueId)), nodeExpIndex(nodeExpIndex), nodeExp(nodeExp) {
        assert(!this->valueId.empty());
        assert(nodeExp);
    }

    // assign_statement : IDENTIFIER LSQUARE exp RSQUARE ASSIGN exp | IDENTIFIER ASSIGN exp
    static shared_ptr<AssignStatement> parse(Scanner &scanner);

    void fillPrettyStrings(std::vector<string> &rows, unsigned indentBase, unsigned indentFactor) override;
};

class IfStatement : public AbstractSyntaxNode {
private:
    shared_ptr<IfStmt> nodeIfStmt;
    shared_ptr<CodeBlock> nodeCodeBlock;

public:
    IfStatement(const shared_ptr<IfStmt> &nodeIfStmt,
                const shared_ptr<CodeBlock> &nodeCodeBlock)
            : nodeIfStmt(nodeIfStmt), nodeCodeBlock(nodeCodeBlock) {
        assert(nodeIfStmt);
    }

    // if_statement : if_stmt | if_stmt ELSE code_block
    static shared_ptr<IfStatement> parse(Scanner &scanner);

    void fillPrettyStrings(std::vector<string> &rows, unsigned indentBase, unsigned indentFactor) override;
};

class IfStmt : public AbstractSyntaxNode {
private:
    shared_ptr<Exp> nodeExp;
    shared_ptr<CodeBlock> nodeCodeBlock;

public:
    IfStmt(const shared_ptr<Exp> &nodeExp, const shared_ptr<CodeBlock> &nodeCodeBlock)
            : nodeExp(nodeExp), nodeCodeBlock(nodeCodeBlock) {
        assert(nodeExp && nodeCodeBlock);
    }

    // if_stmt : IF LPAR exp RPAR code_block
    static shared_ptr<IfStmt> parse(Scanner &scanner);

    void fillPrettyStrings(std::vector<string> &rows, unsigned indentBase, unsigned indentFactor) override;
};

class WhileStatement : public AbstractSyntaxNode {
private:
    shared_ptr<Exp> nodeExp;
    shared_ptr<CodeBlock> nodeCodeBlock;

public:
    WhileStatement(const shared_ptr<Exp> &nodeExp, const shared_ptr<CodeBlock> &nodeCodeBlock)
            : nodeExp(nodeExp), nodeCodeBlock(nodeCodeBlock) {
        assert(nodeExp && nodeCodeBlock);
    }

    // while_statement : WHILE LPAR exp RPAR code_block
    static shared_ptr<WhileStatement> parse(Scanner &scanner);

    void fillPrettyStrings(std::vector<string> &rows, unsigned indentBase, unsigned indentFactor) override;
};

class DoWhileStatement : public AbstractSyntaxNode {
private:
    shared_ptr<CodeBlock> nodeCodeBlock;
    shared_ptr<Exp> nodeExp;

public:
    DoWhileStatement(const shared_ptr<CodeBlock> &nodeCodeBlock, const shared_ptr<Exp> &nodeExp)
            : nodeCodeBlock(nodeCodeBlock), nodeExp(nodeExp) {
        assert(nodeCodeBlock && nodeExp);
    }

    static shared_ptr<DoWhileStatement> parse(Scanner &scanner);

    void fillPrettyStrings(std::vector<string> &rows, unsigned indentBase, unsigned indentFactor) override;
};

class ReturnStatement : public AbstractSyntaxNode {
public:
    ReturnStatement() = default;

    static shared_ptr<ReturnStatement> parse(Scanner &scanner);

    void fillPrettyStrings(std::vector<string> &rows, unsigned indentBase, unsigned indentFactor) override;
};

class ReadStatement : public AbstractSyntaxNode {
private:
    string valueId;

public:
    explicit ReadStatement(string valueId) : valueId(std::move(valueId)) {
        assert(!this->valueId.empty());
    }

    static shared_ptr<ReadStatement> parse(Scanner &scanner);

    void fillPrettyStrings(std::vector<string> &rows, unsigned indentBase, unsigned indentFactor) override;
};

class WriteStatement : public AbstractSyntaxNode {
private:
    shared_ptr<Exp> nodeExp;

public:
    explicit WriteStatement(const shared_ptr<Exp> &nodeExp) : nodeExp(nodeExp) {
        assert(nodeExp);
    }

    static shared_ptr<WriteStatement> parse(Scanner &scanner);

    void fillPrettyStrings(std::vector<string> &rows, unsigned indentBase, unsigned indentFactor) override;
};

class Exp : public AbstractSyntaxNode {
private:
    shared_ptr<Exp2> nodeExp2;
    shared_ptr<Exp> nodeExp;

public:
    Exp(const shared_ptr<Exp2> &nodeExp2, const shared_ptr<Exp> &nodeExp)
            : nodeExp2(nodeExp2), nodeExp(nodeExp) {
        assert(nodeExp2);
    }

    static shared_ptr<Exp> parse(Scanner &scanner);

    void fillPrettyStrings(std::vector<string> &rows, unsigned indentBase, unsigned indentFactor) override;
};

class Exp2 : public AbstractSyntaxNode {
private:
    shared_ptr<Exp3> nodeExp3;
    shared_ptr<Exp2> nodeExp2;

public:
    Exp2(const shared_ptr<Exp3> &nodeExp3, const shared_ptr<Exp2> &nodeExp2)
            : nodeExp3(nodeExp3), nodeExp2(nodeExp2) {
        assert(nodeExp3);
    }

    static shared_ptr<Exp2> parse(Scanner &scanner);

    void fillPrettyStrings(std::vector<string> &rows, unsigned indentBase, unsigned indentFactor) override;
};

class Exp3 : public AbstractSyntaxNode {
private:
    shared_ptr<Exp4> nodeExp4;
    shared_ptr<Exp3> nodeExp3;

public:
    Exp3(const shared_ptr<Exp4> &nodeExp4, const shared_ptr<Exp3> &nodeExp3)
            : nodeExp4(nodeExp4), nodeExp3(nodeExp3) {
        assert(nodeExp4);
    }

    static shared_ptr<Exp3> parse(Scanner &scanner);

    void fillPrettyStrings(std::vector<string> &rows, unsigned indentBase, unsigned indentFactor) override;
};

class Exp4 : public AbstractSyntaxNode {
private:
    shared_ptr<Exp5> nodeExp5;
    shared_ptr<Exp4> nodeExp4;

public:
    Exp4(const shared_ptr<Exp5> &nodeExp5, const shared_ptr<Exp4> &nodeExp4)
            : nodeExp5(nodeExp5), nodeExp4(nodeExp4) {
        assert(nodeExp5);
    }

    static shared_ptr<Exp4> parse(Scanner &scanner);

    void fillPrettyStrings(std::vector<string> &rows, unsigned indentBase, unsigned indentFactor) override;
};

class Exp5 : public AbstractSyntaxNode {
private:
    shared_ptr<Exp6> nodeExp6;
    shared_ptr<Exp5> nodeExp5;
    TokenType tokenOp;

public:
    explicit Exp5(const shared_ptr<Exp6> &nodeExp6)
            : nodeExp6(nodeExp6), nodeExp5(nullptr), tokenOp(INVALID) {
        assert(nodeExp6);
    }

    Exp5(const shared_ptr<Exp6> &nodeExp6, const shared_ptr<Exp5> &nodeExp5, TokenType tokenOp)
            : nodeExp6(nodeExp6), nodeExp5(nodeExp5), tokenOp(tokenOp) {
        assert(nodeExp6);
        assert(!nodeExp5 || tokenOp == EQ || tokenOp == NOTEQ);
    }

    static shared_ptr<Exp5> parse(Scanner &scanner);

    void fillPrettyStrings(std::vector<string> &rows, unsigned indentBase, unsigned indentFactor) override;
};

class Exp6 : public AbstractSyntaxNode {
private:
    shared_ptr<Exp7> nodeExp7;
    shared_ptr<Exp6> nodeExp6;
    TokenType tokenOp;

public:
    explicit Exp6(const shared_ptr<Exp7> &nodeExp7)
            : nodeExp7(nodeExp7), nodeExp6(nullptr), tokenOp(INVALID) {
        assert(nodeExp7);
    }

    Exp6(const shared_ptr<Exp7> &nodeExp7, const shared_ptr<Exp6> &nodeExp6, TokenType tokenOp)
            : nodeExp7(nodeExp7), nodeExp6(nodeExp6), tokenOp(tokenOp) {
        assert(nodeExp7);
        assert(!nodeExp6 || tokenOp == LT || tokenOp == GT || tokenOp == LTEQ || tokenOp == GTEQ);
    }

    static shared_ptr<Exp6> parse(Scanner &scanner);

    void fillPrettyStrings(std::vector<string> &rows, unsigned indentBase, unsigned indentFactor) override;
};

class Exp7 : public AbstractSyntaxNode {
private:
    shared_ptr<Exp8> nodeExp8;
    shared_ptr<Exp7> nodeExp7;
    TokenType tokenOp;

public:
    explicit Exp7(const shared_ptr<Exp8> &nodeExp8)
            : nodeExp8(nodeExp8), nodeExp7(nullptr), tokenOp(INVALID) {
        assert(nodeExp8);
    }

    Exp7(const shared_ptr<Exp8> &nodeExp8, const shared_ptr<Exp7> &nodeExp7, TokenType tokenOp)
            : nodeExp8(nodeExp8), nodeExp7(nodeExp7), tokenOp(tokenOp) {
        assert(nodeExp8);
        assert(!nodeExp7 || tokenOp == SHL_OP || tokenOp == SHR_OP);
    }

    static shared_ptr<Exp7> parse(Scanner &scanner);

    void fillPrettyStrings(std::vector<string> &rows, unsigned indentBase, unsigned indentFactor) override;
};

class Exp8 : public AbstractSyntaxNode {
private:
    shared_ptr<Exp9> nodeExp9;
    shared_ptr<Exp8> nodeExp8;
    TokenType tokenOp;

public:
    explicit Exp8(const shared_ptr<Exp9> &nodeExp9)
            : nodeExp9(nodeExp9), nodeExp8(nullptr), tokenOp(INVALID) {
        assert(nodeExp9);
    }

    Exp8(const shared_ptr<Exp9> &nodeExp9, const shared_ptr<Exp8> &nodeExp8, TokenType tokenOp)
            : nodeExp9(nodeExp9), nodeExp8(nodeExp8), tokenOp(tokenOp) {
        assert(nodeExp9);
        assert(!nodeExp8 || tokenOp == PLUS || tokenOp == MINUS);
    }

    static shared_ptr<Exp8> parse(Scanner &scanner);

    void fillPrettyStrings(std::vector<string> &rows, unsigned indentBase, unsigned indentFactor) override;
};

class Exp9 : public AbstractSyntaxNode {
private:
    shared_ptr<Exp10> nodeExp10;
    shared_ptr<Exp9> nodeExp9;
    TokenType tokenOp;

public:
    explicit Exp9(const shared_ptr<Exp10> &nodeExp10)
            : nodeExp10(nodeExp10), nodeExp9(nullptr), tokenOp(INVALID) {
        assert(nodeExp10);
    }

    Exp9(const shared_ptr<Exp10> &nodeExp10, const shared_ptr<Exp9> &nodeExp9, TokenType tokenOp)
            : nodeExp10(nodeExp10), nodeExp9(nodeExp9), tokenOp(tokenOp) {
        assert(nodeExp10);
        assert(!nodeExp9 || tokenOp == DIV_OP || tokenOp == MUL_OP);
    }

    static shared_ptr<Exp9> parse(Scanner &scanner);

    void fillPrettyStrings(std::vector<string> &rows, unsigned indentBase, unsigned indentFactor) override;
};

class Exp10 : public AbstractSyntaxNode {
private:
    shared_ptr<Exp11> nodeExp11;
    TokenType tokenOp;

public:
    explicit Exp10(const shared_ptr<Exp11> &nodeExp11)
            : nodeExp11(nodeExp11), tokenOp(INVALID) {
        assert(nodeExp11);
    }

    Exp10(const shared_ptr<Exp11> &nodeExp11, TokenType tokenOp)
            : nodeExp11(nodeExp11), tokenOp(tokenOp) {
        assert(nodeExp11);
        assert(tokenOp == INVALID || tokenOp == NOT_OP || tokenOp == MINUS);
    }

    static shared_ptr<Exp10> parse(Scanner &scanner);

    void fillPrettyStrings(std::vector<string> &rows, unsigned indentBase, unsigned indentFactor) override;
};

class Exp11 : public AbstractSyntaxNode {
private:
    int valueNum;
    bool isNum;
    string valueId;
    shared_ptr<Exp> nodeExp;

public:
    explicit Exp11(const int valueNum)
            : valueNum(valueNum), isNum(true), nodeExp(nullptr) {
    }

    explicit Exp11(const shared_ptr<Exp> &nodeExp)
            : valueNum(0), isNum(false), nodeExp(nodeExp) {
        assert(nodeExp);
    }

    Exp11(string valueId, const shared_ptr<Exp> &nodeExp)
            : valueNum(0), isNum(false), valueId(std::move(valueId)), nodeExp(nodeExp) {
        assert(!this->valueId.empty() || nodeExp);
    }

    static shared_ptr<Exp11> parse(Scanner &scanner);

    void fillPrettyStrings(std::vector<string> &rows, unsigned indentBase, unsigned indentFactor) override;
};


#endif //SIMPLE_COMPILER_FOR_MIPS_PARSER_H

