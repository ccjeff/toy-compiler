//
// Created by ccjeff on 2021/4/7.
//
#include "parser.h"
#include <iostream>

// begin the parser routine
shared_ptr<Program> Program::parse(Scanner &scanner, Translator &translator) {
    if (scanner.hasToken()) {
        shared_ptr<VarDeclarations> nodeVarDeclarations;
        shared_ptr<Statements> nodeStatements;

        switch (scanner.getTokenType()) {
            case ID:
            case IF:
            case WHILE:
            case DO:
            case RETURN:
            case READ:
            case WRITE:
            case SEMI:
            case INT:
                nodeVarDeclarations = VarDeclarations::parse(scanner, translator);
                nodeStatements = Statements::parse(scanner, translator);
                return make_shared<Program>(nodeVarDeclarations,
                                            nodeStatements);
            default:
                throw UnexpectedTokenException(scanner.getTokenValue());
        }
    } else {
        throw UnexpectedTerminationException();
    }
}

void Program::fillPrettyStrings(std::vector<string> &rows, unsigned indentBase, unsigned indentFactor) {
    rows.push_back(string(indentBase * indentFactor, ' ') + "program");
    nodeVarDeclarations->fillPrettyStrings(rows, indentBase, indentFactor + 1);
    nodeStatements->fillPrettyStrings(rows, indentBase, indentFactor + 1);
}

shared_ptr<VarDeclarations> VarDeclarations::parse(Scanner &scanner, Translator &translator) {
    if (scanner.hasToken() && scanner.getTokenType() == INT) {
        shared_ptr<VarDeclaration> nodeVarDeclaration = VarDeclaration::parse(scanner, translator);
        if (scanner.hasToken() && scanner.getTokenType() == INT) {
            return make_shared<VarDeclarations>(
                    nodeVarDeclaration,
                    VarDeclarations::parse(scanner, translator)
            );
        } else {
            return make_shared<VarDeclarations>(nodeVarDeclaration, nullptr);
        }
    } else {
        return make_shared<VarDeclarations>(nullptr, nullptr);
    }
}

void VarDeclarations::fillPrettyStrings(std::vector<string> &rows, unsigned indentBase, unsigned indentFactor) {
    rows.push_back(string(indentBase * indentFactor, ' ') + "var_declarations");
    if (nodeVarDeclaration) {
        nodeVarDeclaration->fillPrettyStrings(rows, indentBase, indentFactor + 1);
    }
    if (nodeVarDeclarations) {
        nodeVarDeclarations->fillPrettyStrings(rows, indentBase, indentFactor + 1);
    }
}

shared_ptr<VarDeclaration> VarDeclaration::parse(Scanner &scanner, Translator &translator) {
    validateTokenType(scanner, INT);
    scanner.nextToken();

    shared_ptr<DeclarationList> nodeDeclarationList = DeclarationList::parse(scanner, translator);

    validateTokenType(scanner, SEMI);
    scanner.nextToken();

    return make_shared<VarDeclaration>(nodeDeclarationList);
}

void VarDeclaration::fillPrettyStrings(std::vector<string> &rows, unsigned indentBase, unsigned indentFactor) {
    rows.push_back(string(indentBase * indentFactor, ' ') + "var_declaration");
    nodeDeclarationList->fillPrettyStrings(rows, indentBase, indentFactor + 1);
}

shared_ptr<DeclarationList> DeclarationList::parse(Scanner &scanner, Translator &translator) {
    shared_ptr<Declaration> nodeDeclaration = Declaration::parse(scanner, translator);
    if (scanner.hasToken() && scanner.getTokenType() == COMMA) {
        scanner.nextToken();
        return make_shared<DeclarationList>(
                nodeDeclaration,
                DeclarationList::parse(scanner, translator)
        );
    } else {
        return make_shared<DeclarationList>(nodeDeclaration, nullptr);
    }
}

void DeclarationList::fillPrettyStrings(std::vector<string> &rows, unsigned indentBase, unsigned indentFactor) {
    rows.push_back(string(indentBase * indentFactor, ' ') + "declaration_list");
    nodeDeclaration->fillPrettyStrings(rows, indentBase, indentFactor + 1);
    if (nodeDeclarationList) {
        nodeDeclarationList->fillPrettyStrings(rows, indentBase, indentFactor + 1);
    }
}

shared_ptr<Declaration> Declaration::parse(Scanner &scanner, Translator &translator) {
    validateTokenType(scanner, ID);
    std::string valueId(scanner.getTokenValue());
    scanner.nextToken();

    if (scanner.hasToken()) {
        int valueNumber;
        switch (scanner.getTokenType()) {
            case ASSIGN:
                scanner.nextToken();
                validateTokenType(scanner, INT_NUM);
                valueNumber = atoi(scanner.getTokenValue().c_str());
                // CODEGEN routine
                translator.updateTable(valueId, valueNumber);

                scanner.nextToken();

                return make_shared<Declaration>(
                        valueId,
                        valueNumber,
                        false
                );
            case LSQUARE:
                scanner.nextToken();
                validateTokenType(scanner, INT_NUM);
                valueNumber = atoi(scanner.getTokenValue().c_str());
                translator.updateTable(valueId, valueNumber);
                scanner.nextToken();
                validateTokenType(scanner, RSQUARE);
                scanner.nextToken();
                return make_shared<Declaration>(
                        valueId,
                        valueNumber,
                        true
                );
            default:
                //
//                std::cout << "---CODEGEN--- : int aaa;" << std::endl;
                translator.updateTable(valueId,0);
                return make_shared<Declaration>(valueId);
        }
    } else {
        return make_shared<Declaration>(valueId);
    }
}

void Declaration::fillPrettyStrings(std::vector<string> &rows, unsigned int indentBase, unsigned int indentFactor) {
    rows.push_back(string(indentBase * indentFactor, ' ') + "declaration {id=" + valueId + "}");
    // TODO: In detail
}

shared_ptr<CodeBlock> CodeBlock::parse(Scanner &scanner, Translator &translator) {
    if (scanner.hasToken()) {
        shared_ptr<Statements> nodeStatements;

        switch (scanner.getTokenType()) {
            case LBRACE:
                scanner.nextToken();
                nodeStatements = Statements::parse(scanner, translator);
                validateTokenType(scanner, RBRACE);
                scanner.nextToken();
                return make_shared<CodeBlock>(nodeStatements);
            default:
                return make_shared<CodeBlock>(Statement::parse(scanner, translator));
        }
    } else {
        throw UnexpectedTerminationException();
    }
}

void CodeBlock::fillPrettyStrings(std::vector<string> &rows, unsigned int indentBase, unsigned int indentFactor) {
    rows.push_back(string(indentBase * indentFactor, ' ') + "code_block");
    if (nodeStatement) {
        nodeStatement->fillPrettyStrings(rows, indentBase, indentFactor + 1);
    }
    if (nodeStatements) {
        nodeStatements->fillPrettyStrings(rows, indentBase, indentFactor + 1);
    }
}

shared_ptr<Statements> Statements::parse(Scanner &scanner, Translator &translator) {
    shared_ptr<Statement> nodeStatement = Statement::parse(scanner, translator);
    if (scanner.hasToken()) {
        switch (scanner.getTokenType()) {
            case ID:
            case IF:
            case WHILE:
            case DO:
            case RETURN:
            case READ:
            case WRITE:
            case SEMI:
                return make_shared<Statements>(
                        nodeStatement,
                        Statements::parse(scanner, translator)
                );
            default:
                return make_shared<Statements>(nodeStatement, nullptr);
        }
    } else {
        return make_shared<Statements>(nodeStatement, nullptr);
    }
}

void Statements::fillPrettyStrings(std::vector<string> &rows, unsigned int indentBase, unsigned int indentFactor) {
    rows.push_back(string(indentBase * indentFactor, ' ') + "statements");

    nodeStatement->fillPrettyStrings(rows, indentBase, indentFactor + 1);
    if (nodeStatements) {
        nodeStatements->fillPrettyStrings(rows, indentBase, indentFactor + 1);
    }
}

shared_ptr<Statement> Statement::parse(Scanner &scanner, Translator &translator) {
    if (scanner.hasToken()) {
        shared_ptr<AssignStatement> nodeAssignStatement;
        shared_ptr<ReadWriteStatement> nodeReadWriteStatement;

        switch (scanner.getTokenType()) {
            case ID:
                nodeAssignStatement = AssignStatement::parse(scanner, translator);
                validateTokenType(scanner, SEMI);
                scanner.nextToken();
                return make_shared<Statement>(nodeAssignStatement);
            case IF:
            case WHILE:
            case DO:
            case RETURN:
                return make_shared<Statement>(ControlStatement::parse(scanner, translator));
            case READ:
            case WRITE:
                nodeReadWriteStatement = ReadWriteStatement::parse(scanner, translator);
                validateTokenType(scanner, SEMI);
                scanner.nextToken();
                return make_shared<Statement>(nodeReadWriteStatement);
            case SEMI:
                scanner.nextToken();
                return make_shared<Statement>();
            default:
                throw UnexpectedTokenException(scanner.getTokenValue());
        }
    } else {
        throw UnexpectedTerminationException();
    }
}

void Statement::fillPrettyStrings(std::vector<string> &rows, unsigned int indentBase, unsigned int indentFactor) {
    rows.push_back(string(indentBase * indentFactor, ' ') + "statement");

    if (nodeAssignStatement) {
        nodeAssignStatement->fillPrettyStrings(rows, indentBase, indentFactor + 1);
    } else if (nodeControlStatement) {
        nodeControlStatement->fillPrettyStrings(rows, indentBase, indentFactor + 1);
    } else if (nodeReadWriteStatement) {
        nodeReadWriteStatement->fillPrettyStrings(rows, indentBase, indentFactor + 1);
    }
}

shared_ptr<ControlStatement> ControlStatement::parse(Scanner &scanner, Translator &translator) {
    if (scanner.hasToken()) {
        shared_ptr<DoWhileStatement> nodeDoWhileStatement;
        shared_ptr<ReturnStatement> nodeReturnStatement;

        switch (scanner.getTokenType()) {
            case IF:
                return make_shared<ControlStatement>(IfStatement::parse(scanner, translator));
            case WHILE:
                return make_shared<ControlStatement>(WhileStatement::parse(scanner, translator));
            case DO:
                nodeDoWhileStatement = DoWhileStatement::parse(scanner, translator);
                validateTokenType(scanner, SEMI);
                scanner.nextToken();
                return make_shared<ControlStatement>(nodeDoWhileStatement);
            case RETURN:
                nodeReturnStatement = ReturnStatement::parse(scanner, translator);
                validateTokenType(scanner, SEMI);
                scanner.nextToken();
                return make_shared<ControlStatement>(nodeReturnStatement);
            default:
                throw UnexpectedTokenException(scanner.getTokenValue());
        }
    } else {
        throw UnexpectedTerminationException();
    }
}

void
ControlStatement::fillPrettyStrings(std::vector<string> &rows, unsigned int indentBase, unsigned int indentFactor) {
    rows.push_back(string(indentBase * indentFactor, ' ') + "control_statement");

    if (nodeIfStatement) {
        nodeIfStatement->fillPrettyStrings(rows, indentBase, indentFactor + 1);
    } else if (nodeWhileStatement) {
        nodeWhileStatement->fillPrettyStrings(rows, indentBase, indentFactor + 1);
    } else if (nodeDoWhileStatement) {
        nodeDoWhileStatement->fillPrettyStrings(rows, indentBase, indentFactor + 1);
    } else if (nodeReturnStatement) {
        nodeReturnStatement->fillPrettyStrings(rows, indentBase, indentFactor + 1);
    }
}

shared_ptr<ReadWriteStatement> ReadWriteStatement::parse(Scanner &scanner, Translator &translator) {
    if (scanner.hasToken()) {
        switch (scanner.getTokenType()) {
            case READ:
                return make_shared<ReadWriteStatement>(ReadStatement::parse(scanner, translator));
            case WRITE:
                return make_shared<ReadWriteStatement>(WriteStatement::parse(scanner, translator));
            default:
                throw UnexpectedTokenException(scanner.getTokenValue());
        }
    } else {
        throw UnexpectedTerminationException();
    }
}

void
ReadWriteStatement::fillPrettyStrings(std::vector<string> &rows, unsigned int indentBase, unsigned int indentFactor) {
    rows.push_back(string(indentBase * indentFactor, ' ') + "read_write_statement");

    if (nodeReadStatement) {
        nodeReadStatement->fillPrettyStrings(rows, indentBase, indentFactor + 1);
    } else if (nodeWriteStatement) {
        nodeWriteStatement->fillPrettyStrings(rows, indentBase, indentFactor + 1);
    }
}

shared_ptr<AssignStatement> AssignStatement::parse(Scanner &scanner, Translator &translator) {
    validateTokenType(scanner, ID);
    std::string valueId(scanner.getTokenValue());
    scanner.nextToken();
    if (scanner.hasToken()) {
        shared_ptr<Exp> nodeExpIndex;
        if (scanner.getTokenType() == LSQUARE) {
            scanner.nextToken();
            nodeExpIndex = Exp::parse(scanner, translator);
            validateTokenType(scanner, RSQUARE);
            scanner.nextToken();
            validateTokenType(scanner, ASSIGN);
            scanner.nextToken();
            return make_shared<AssignStatement>(valueId, nodeExpIndex, Exp::parse(scanner, translator));
        } else if (scanner.getTokenType() == ASSIGN){
            scanner.nextToken();
            auto res = make_shared<AssignStatement>(valueId, nullptr, Exp::parse(scanner, translator));
            std::cout << "Store" << "," << valueId << ",";
            if (translator.lastTemp != "") std::cout << translator.lastTemp << std::endl;
            else std::cout << translator.last2Assign << std::endl;
            translator.lastTemp = "";
            translator.last2Assign = -1;
            // TODO: should check for entry before doing so
            return res;
        }
    } else {
        throw UnexpectedTerminationException();
    }
    throw UnexpectedTokenException(scanner.getTokenValue());
}

void AssignStatement::fillPrettyStrings(std::vector<string> &rows, unsigned int indentBase, unsigned int indentFactor) {
    rows.push_back(string(indentBase * indentFactor, ' ') + "assign_statement {id=" + valueId + "}");
    if (nodeExpIndex) {
        nodeExpIndex->fillPrettyStrings(rows, indentBase, indentFactor + 1);
    }
    nodeExp->fillPrettyStrings(rows, indentBase, indentFactor + 1);
}

shared_ptr<IfStatement> IfStatement::parse(Scanner &scanner, Translator &translator) {
    shared_ptr<IfStmt> nodeIfStmt = IfStmt::parse(scanner, translator);
    if (scanner.hasToken() && scanner.getTokenType() == ELSE) {
        scanner.nextToken();
        std::cout << "Else" << std::endl;
        auto res = make_shared<IfStatement>(nodeIfStmt, CodeBlock::parse(scanner, translator));

        return res;
    } else {
        auto res = make_shared<IfStatement>(nodeIfStmt, nullptr);
        // no else
        return res;
    }
}

void IfStatement::fillPrettyStrings(std::vector<string> &rows, unsigned int indentBase, unsigned int indentFactor) {
    rows.push_back(string(indentBase * indentFactor, ' ') + "if_statement");
    nodeIfStmt->fillPrettyStrings(rows, indentBase, indentFactor + 1);
    if (nodeCodeBlock) {
        nodeCodeBlock->fillPrettyStrings(rows, indentBase, indentFactor + 1);
    }
}

shared_ptr<IfStmt> IfStmt::parse(Scanner &scanner, Translator &translator) {
    validateTokenType(scanner, IF);
    scanner.nextToken();

    validateTokenType(scanner, LPAR);
    scanner.nextToken();
    std::cout << "If" << ",";
    shared_ptr<Exp> nodeExp = Exp::parse(scanner, translator);
    std::cout << translator.lastID << ",";
    if (translator.last2Assign != -1) std::cout << translator.last2Assign << std::endl;
    else std::cout << translator.lastID << std::endl;
    translator.lastID = "";
    translator.last2Assign = -1;
    validateTokenType(scanner, RPAR);
    scanner.nextToken();

    shared_ptr<CodeBlock> nodeCodeBlock = CodeBlock::parse(scanner, translator);
    std::cout << "END IF" << std::endl;
    return make_shared<IfStmt>(nodeExp, nodeCodeBlock);
}

void IfStmt::fillPrettyStrings(std::vector<string> &rows, unsigned int indentBase, unsigned int indentFactor) {
    rows.push_back(string(indentBase * indentFactor, ' ') + "if_stmt");

    nodeExp->fillPrettyStrings(rows, indentBase, indentFactor + 1);
    nodeCodeBlock->fillPrettyStrings(rows, indentBase, indentFactor + 1);
}

shared_ptr<WhileStatement> WhileStatement::parse(Scanner &scanner, Translator &translator) {
    validateTokenType(scanner, WHILE);
    scanner.nextToken();

    validateTokenType(scanner, LPAR);
    scanner.nextToken();
    std::cout << "While" << ",";
    shared_ptr<Exp> nodeExp = Exp::parse(scanner, translator);

    validateTokenType(scanner, RPAR);
    scanner.nextToken();

    shared_ptr<CodeBlock> nodeCodeBlock = CodeBlock::parse(scanner,translator);

    auto res = make_shared<WhileStatement>(nodeExp, nodeCodeBlock);
    std::cout << "WHILE END" << std::endl;
    return res;
}

void WhileStatement::fillPrettyStrings(std::vector<string> &rows, unsigned int indentBase, unsigned int indentFactor) {
    rows.push_back(string(indentBase * indentFactor, ' ') + "while_statement");

    nodeExp->fillPrettyStrings(rows, indentBase, indentFactor + 1);
    nodeCodeBlock->fillPrettyStrings(rows, indentBase, indentFactor + 1);
}

shared_ptr<DoWhileStatement> DoWhileStatement::parse(Scanner &scanner, Translator &translator) {
    validateTokenType(scanner, DO);
    scanner.nextToken();

    shared_ptr<CodeBlock> nodeCodeBlock = CodeBlock::parse(scanner, translator);

    validateTokenType(scanner, WHILE);
    scanner.nextToken();

    validateTokenType(scanner, LPAR);
    scanner.nextToken();

    shared_ptr<Exp> nodeExp = Exp::parse(scanner,translator);

    validateTokenType(scanner, RPAR);
    scanner.nextToken();

    return make_shared<DoWhileStatement>(nodeCodeBlock, nodeExp);
}

void
DoWhileStatement::fillPrettyStrings(std::vector<string> &rows, unsigned int indentBase, unsigned int indentFactor) {
    rows.push_back(string(indentBase * indentFactor, ' ') + "do_while_statement");

    nodeCodeBlock->fillPrettyStrings(rows, indentBase, indentFactor + 1);
    nodeExp->fillPrettyStrings(rows, indentBase, indentFactor + 1);
}

shared_ptr<ReturnStatement> ReturnStatement::parse(Scanner &scanner, Translator &translator) {
    validateTokenType(scanner, RETURN);
    scanner.nextToken();

    return make_shared<ReturnStatement>();
}

void ReturnStatement::fillPrettyStrings(std::vector<string> &rows, unsigned int indentBase, unsigned int indentFactor) {
    rows.push_back(string(indentBase * indentFactor, ' ') + "return_statement");
}

shared_ptr<ReadStatement> ReadStatement::parse(Scanner &scanner, Translator &translator) {
    validateTokenType(scanner, READ);
    scanner.nextToken();

    validateTokenType(scanner, LPAR);
    scanner.nextToken();

    validateTokenType(scanner, ID);
    string valueId(scanner.getTokenValue());
    std::cout << "Read" << "," << valueId << std::endl;
    scanner.nextToken();

    validateTokenType(scanner, RPAR);
    scanner.nextToken();

    return make_shared<ReadStatement>(valueId);
}

void ReadStatement::fillPrettyStrings(std::vector<string> &rows, unsigned int indentBase, unsigned int indentFactor) {
    rows.push_back(string(indentBase * indentFactor, ' ') + "read_statement {id=" + valueId + "}");
}

shared_ptr<WriteStatement> WriteStatement::parse(Scanner &scanner, Translator &translator) {
    validateTokenType(scanner, WRITE);
    scanner.nextToken();

    validateTokenType(scanner, LPAR);
    scanner.nextToken();
    std::cout << "Write" << ",";
    shared_ptr<Exp> nodeExp = Exp::parse(scanner, translator);
    std::cout << translator.lastID << std::endl;
    translator.lastID = "";
    validateTokenType(scanner, RPAR);
    scanner.nextToken();

    return make_shared<WriteStatement>(nodeExp);
}

void WriteStatement::fillPrettyStrings(std::vector<string> &rows, unsigned int indentBase, unsigned int indentFactor) {
    rows.push_back(string(indentBase * indentFactor, ' ') + "write_statement");

    nodeExp->fillPrettyStrings(rows, indentBase, indentFactor + 1);
}

shared_ptr<Exp> Exp::parse(Scanner &scanner, Translator &translator) {
    shared_ptr<Exp2> nodeExp2 = Exp2::parse(scanner, translator);

    if (scanner.hasToken() && scanner.getTokenType() == OROR) {
        scanner.nextToken();
        return make_shared<Exp>(nodeExp2, Exp::parse(scanner, translator));
    } else {
        return make_shared<Exp>(nodeExp2, nullptr);
    }
}

void Exp::fillPrettyStrings(std::vector<string> &rows, unsigned int indentBase, unsigned int indentFactor) {
    rows.push_back(string(indentBase * indentFactor, ' ') + "exp");

    nodeExp2->fillPrettyStrings(rows, indentBase, indentFactor + 1);
    if (nodeExp) {
        nodeExp->fillPrettyStrings(rows, indentBase, indentFactor + 1);
    }
}

shared_ptr<Exp2> Exp2::parse(Scanner &scanner, Translator &translator) {
    shared_ptr<Exp3> nodeExp3 = Exp3::parse(scanner, translator);

    if (scanner.hasToken() && scanner.getTokenType() == ANDAND) {
        scanner.nextToken();
        return make_shared<Exp2>(nodeExp3, Exp2::parse(scanner, translator));
    } else {
        return make_shared<Exp2>(nodeExp3, nullptr);
    }
}

void Exp2::fillPrettyStrings(std::vector<string> &rows, unsigned int indentBase, unsigned int indentFactor) {
    rows.push_back(string(indentBase * indentFactor, ' ') + "exp2");

    nodeExp3->fillPrettyStrings(rows, indentBase, indentFactor + 1);
    if (nodeExp2) {
        nodeExp2->fillPrettyStrings(rows, indentBase, indentFactor + 1);
    }
}

shared_ptr<Exp3> Exp3::parse(Scanner &scanner, Translator &translator) {
    shared_ptr<Exp4> nodeExp4 = Exp4::parse(scanner, translator);

    if (scanner.hasToken() && scanner.getTokenType() == OR_OP) {
        scanner.nextToken();
        return make_shared<Exp3>(nodeExp4, Exp3::parse(scanner, translator));
    } else {
        return make_shared<Exp3>(nodeExp4, nullptr);
    }
}

void Exp3::fillPrettyStrings(std::vector<string> &rows, unsigned int indentBase, unsigned int indentFactor) {
    rows.push_back(string(indentBase * indentFactor, ' ') + "exp3");

    nodeExp4->fillPrettyStrings(rows, indentBase, indentFactor + 1);
    if (nodeExp3) {
        nodeExp3->fillPrettyStrings(rows, indentBase, indentFactor + 1);
    }
}

shared_ptr<Exp4> Exp4::parse(Scanner &scanner, Translator &translator) {
    shared_ptr<Exp5> nodeExp5 = Exp5::parse(scanner, translator);

    if (scanner.hasToken() && scanner.getTokenType() == AND_OP) {
        scanner.nextToken();
        return make_shared<Exp4>(nodeExp5, Exp4::parse(scanner, translator));
    } else {
        return make_shared<Exp4>(nodeExp5, nullptr);
    }
}

void Exp4::fillPrettyStrings(std::vector<string> &rows, unsigned int indentBase, unsigned int indentFactor) {
    rows.push_back(string(indentBase * indentFactor, ' ') + "exp4");

    nodeExp5->fillPrettyStrings(rows, indentBase, indentFactor + 1);
    if (nodeExp4) {
        nodeExp4->fillPrettyStrings(rows, indentBase, indentFactor + 1);
    }
}

shared_ptr<Exp5> Exp5::parse(Scanner &scanner, Translator &translator) {
    shared_ptr<Exp6> nodeExp6 = Exp6::parse(scanner, translator);

    if (scanner.hasToken()) {
        TokenType tokenOp = scanner.getTokenType();
        if (tokenOp == EQ || tokenOp == NOTEQ) {
            scanner.nextToken();
            auto res = make_shared<Exp5>(nodeExp6, Exp5::parse(scanner, translator), tokenOp);
            std::cout << token2string(tokenOp) << "," << translator.lastID << "," << translator.last2Assign << std::endl;
            translator.lastID = "";
            translator.last2Assign = -1;
            return res;
        }
    } else {
        return make_shared<Exp5>(nodeExp6);
    }
    return make_shared<Exp5>(nodeExp6);
}

void Exp5::fillPrettyStrings(std::vector<string> &rows, unsigned int indentBase, unsigned int indentFactor) {
    if (nodeExp5) {
        rows.push_back(string(indentBase * indentFactor, ' ') + "exp5 {" + token2string(tokenOp) + "}");
    } else {
        rows.push_back(string(indentBase * indentFactor, ' ') + "exp5");
    }

    nodeExp6->fillPrettyStrings(rows, indentBase, indentFactor + 1);
    if (nodeExp5) {
        nodeExp5->fillPrettyStrings(rows, indentBase, indentFactor + 1);
    }
}

shared_ptr<Exp6> Exp6::parse(Scanner &scanner, Translator &translator) {
    shared_ptr<Exp7> nodeExp7 = Exp7::parse(scanner, translator);

    if (scanner.hasToken()) {
        TokenType tokenOp = scanner.getTokenType();
        if (tokenOp == LT || tokenOp == GT || tokenOp == LTEQ || tokenOp == GTEQ) {
            scanner.nextToken();
            std::cout << token2string(tokenOp) << ",";
            auto res = make_shared<Exp6>(nodeExp7, Exp6::parse(scanner, translator), tokenOp);
            return res;
        }
    } else {
        return make_shared<Exp6>(nodeExp7);
    }
    return make_shared<Exp6>(nodeExp7);
}

void Exp6::fillPrettyStrings(std::vector<string> &rows, unsigned int indentBase, unsigned int indentFactor) {
    if (nodeExp6) {
        rows.push_back(string(indentBase * indentFactor, ' ') + "exp6 {" + token2string(tokenOp) + "}");
    } else {
        rows.push_back(string(indentBase * indentFactor, ' ') + "exp6");
    }

    nodeExp7->fillPrettyStrings(rows, indentBase, indentFactor + 1);
    if (nodeExp6) {
        nodeExp6->fillPrettyStrings(rows, indentBase, indentFactor + 1);
    }
}

shared_ptr<Exp7> Exp7::parse(Scanner &scanner, Translator &translator) {
    shared_ptr<Exp8> nodeExp8 = Exp8::parse(scanner, translator);

    if (scanner.hasToken()) {
        TokenType tokenOp = scanner.getTokenType();
        switch (tokenOp) {
            case SHL_OP:
            case SHR_OP:
                scanner.nextToken();
                return make_shared<Exp7>(nodeExp8, Exp7::parse(scanner, translator), tokenOp);
            default:
                return make_shared<Exp7>(nodeExp8);
        }
    } else {
        return make_shared<Exp7>(nodeExp8);
    }
}

void Exp7::fillPrettyStrings(std::vector<string> &rows, unsigned int indentBase, unsigned int indentFactor) {
    if (nodeExp7) {
        rows.push_back(string(indentBase * indentFactor, ' ') + "exp7 {" + token2string(tokenOp) + "}");
    } else {
        rows.push_back(string(indentBase * indentFactor, ' ') + "exp7");
    }

    nodeExp8->fillPrettyStrings(rows, indentBase, indentFactor + 1);
    if (nodeExp7) {
        nodeExp7->fillPrettyStrings(rows, indentBase, indentFactor + 1);
    }
}

shared_ptr<Exp8> Exp8::parse(Scanner &scanner, Translator &translator) {
    shared_ptr<Exp9> nodeExp9 = Exp9::parse(scanner, translator);

    if (scanner.hasToken()) {
        TokenType tokenOp = scanner.getTokenType();
        string tempName;
        if (tokenOp == PLUS) {
            tempName = translator.getTemp();
            scanner.nextToken();
            std::cout << "Add" << "," << tempName << ",";
            if (translator.last2Assign != -1) std::cout << translator.last2Assign << ",";
            else std::cout << translator.lastID << ",";
            translator.lastTemp = tempName;
            auto res8 = make_shared<Exp8>(nodeExp9, Exp8::parse(scanner, translator), tokenOp);
            if (translator.last2Assign != -1) std::cout << translator.last2Assign << std::endl;
            else std::cout << translator.lastID << std::endl;
            translator.last2Assign = -1;
            translator.lastID = "";
            return res8;
        } else if (tokenOp == MINUS){
            tempName = translator.getTemp();
            scanner.nextToken();
            std::cout << "Min" << "," << tempName << "," << translator.last2Assign << ",";
            if (translator.last2Assign != -1) std::cout << translator.last2Assign << ",";
            else std::cout << translator.lastID << ",";
            translator.lastTemp = tempName;
            auto res8 = make_shared<Exp8>(nodeExp9, Exp8::parse(scanner, translator), tokenOp);
            if (translator.last2Assign != -1) std::cout << translator.last2Assign << std::endl;
            else std::cout << translator.lastID << std::endl;
            translator.last2Assign = -1;
            translator.lastID = "";
            return res8;
        } else {
            return make_shared<Exp8>(nodeExp9);
        }
    } else {
        return make_shared<Exp8>(nodeExp9);
    }
}

void Exp8::fillPrettyStrings(std::vector<string> &rows, unsigned int indentBase, unsigned int indentFactor) {
    if (nodeExp8) {
        rows.push_back(string(indentBase * indentFactor, ' ') + "exp8 {" + token2string(tokenOp) + "}");
    } else {
        rows.push_back(string(indentBase * indentFactor, ' ') + "exp8");
    }

    nodeExp9->fillPrettyStrings(rows, indentBase, indentFactor + 1);
    if (nodeExp8) {
        nodeExp8->fillPrettyStrings(rows, indentBase, indentFactor + 1);
    }
}

shared_ptr<Exp9> Exp9::parse(Scanner &scanner, Translator &translator) {
    shared_ptr<Exp10> nodeExp10 = Exp10::parse(scanner, translator);

    if (scanner.hasToken()) {
        TokenType tokenOp = scanner.getTokenType();
        switch (tokenOp) {
            case DIV_OP:
            case MUL_OP:
                scanner.nextToken();
                return make_shared<Exp9>(nodeExp10, Exp9::parse(scanner, translator), tokenOp);
            default:
                return make_shared<Exp9>(nodeExp10);
        }
    } else {
        return make_shared<Exp9>(nodeExp10);
    }
}

void Exp9::fillPrettyStrings(std::vector<string> &rows, unsigned int indentBase, unsigned int indentFactor) {
    if (nodeExp9) {
        rows.push_back(string(indentBase * indentFactor, ' ') + "exp9 {" + token2string(tokenOp) + "}");
    } else {
        rows.push_back(string(indentBase * indentFactor, ' ') + "exp9");
    }

    nodeExp10->fillPrettyStrings(rows, indentBase, indentFactor + 1);
    if (nodeExp9) {
        nodeExp9->fillPrettyStrings(rows, indentBase, indentFactor + 1);
    }
}

shared_ptr<Exp10> Exp10::parse(Scanner &scanner, Translator &translator) {
    if (scanner.hasToken()) {
        TokenType tokenOp = scanner.getTokenType();
        switch (tokenOp) {
            case NOT_OP:
            case MINUS:
                scanner.nextToken();
                return make_shared<Exp10>(Exp11::parse(scanner, translator), tokenOp);
            default:
                return make_shared<Exp10>(Exp11::parse(scanner, translator));
        }
    } else {
        throw UnexpectedTerminationException();
    }
}

void Exp10::fillPrettyStrings(std::vector<string> &rows, unsigned int indentBase, unsigned int indentFactor) {
    if (tokenOp == INVALID) {
        rows.push_back(string(indentBase * indentFactor, ' ') + "exp10");
    } else {
        rows.push_back(string(indentBase * indentFactor, ' ') + "exp10 {" + token2string(tokenOp) + "}");
    }

    nodeExp11->fillPrettyStrings(rows, indentBase, indentFactor + 1);
}

shared_ptr<Exp11> Exp11::parse(Scanner &scanner, Translator &translator) {
    if (scanner.hasToken()) {
        int valueNumber;
        string valueId;
        shared_ptr<Exp> nodeExp;

        switch (scanner.getTokenType()) {
            case INT_NUM:
                valueNumber = atoi(scanner.getTokenValue().c_str());
                translator.last2Assign = valueNumber;
//                std::cout << "DEBUG: " << valueNumber << std::endl;
                scanner.nextToken();
                return make_shared<Exp11>(valueNumber);
            case ID:
                valueId = scanner.getTokenValue();
//                std::cout << valueId << std::endl;
                translator.lastID = valueId;
                scanner.nextToken();
                if (scanner.hasToken() && scanner.getTokenType() == LSQUARE) {
                    scanner.nextToken();
                    nodeExp = Exp::parse(scanner, translator);
                    validateTokenType(scanner, RSQUARE);
                    scanner.nextToken();
                    return make_shared<Exp11>(valueId, nodeExp);
                } else {
                    return make_shared<Exp11>(valueId, nullptr);
                }
            case LPAR:
                scanner.nextToken();
                nodeExp = Exp::parse(scanner, translator);
                validateTokenType(scanner, RPAR);
                scanner.nextToken();
                return make_shared<Exp11>(nodeExp);
            default:
                throw UnexpectedTokenException(scanner.getTokenValue());
        }
    } else {
        throw UnexpectedTerminationException();
    }
}

void Exp11::fillPrettyStrings(std::vector<string> &rows, unsigned int indentBase, unsigned int indentFactor) {
    if (isNum) {
        rows.push_back(string(indentBase * indentFactor, ' ') + "exp11 {num=" + std::to_string(valueNum) + "}");
    } else {
        rows.push_back(string(indentBase * indentFactor, ' ') + "exp11 {id=" + valueId + "}");
        if (nodeExp) {
            nodeExp->fillPrettyStrings(rows, indentBase, indentFactor + 1);
        }
    }
}
