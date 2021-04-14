# Parser for Simplified C language
117010008 Zhengjia Cao

### 1. Purpose

In this project, I have written a parser for the simplified C language. The parser is a part of the compiler front-end. It's role is to take the input `tokens` from the scanner, and return an abstract syntax tree (AST) using the pre-defined grammar rules as the output.  Unlike the YACC or the GNU Bison, this compiler is just a compiler for the given context free grammar (CFG). If the grammar is changed, the parser will fail to do its job. The parser I have implemented for this project is a LL(1) parser using the recursive descend parsing schema. However, in the PDF file, the CFG was specified for a LALR parser. This have been proved to be causing tons of problem as it does not pass the LL(1) grammar requirements. I have made modification to the CFG given in the assignment PDF to pass the LL(1) grammar requirements. The details of the CFG modifications and methods will be covered in the later sections.

Before we start to discuss about the details of the implementation, I would like to introduce again what a parser is doing and how the parser is defferent from yacc, the parser generator. Yacc (Yet Another Compiler-Compiler) is a computer program for the Unix operating system developed by Stephen C. Johnson. It is a Look Ahead Left-to-Right (LALR) parser generator, generating a LALR parser (the part of a compiler that tries to make syntactic sense of the source code) based on a formal grammar, written in a notation similar to Backus–Naur Form (BNF). Yacc is supplied as a standard utility on BSD and AT&T Unix. GNU-based Linux distributions include Bison, a forward-compatible Yacc replacement. The input to Yacc is a grammar with snippets of C code (called "actions") attached to its rules. Its output is a shift-reduce parser in C that executes the C snippets associated with each rule as soon as the rule is recognized. Typical actions involve the construction of parse trees. 

However, a LR parser is hard to implement by hand. Moreover, our grammar rules are fixed. Therefore, it is unnecessary to use LR parser if we can find an efficient method to change the given CFG into a LL(1) recognizable grammar. Moreover, the convertion to LL(1) grammar can be done prior to runtime, which won't sacrifice much system performance. After a trade-off between development efficiency and generalizability, I have chosen to implement a LL(1) parser.



### 2. Method and Design

In this section, I will first describe some of the methods I have used to make the CFG posted following the LL(1) standard. I will also be discussing the design of the compiler itself in this section.

##### 2.1 Remove Left Recursion

Generally, for a CFG, there will exist two kinds of left recursion: *Immediate Left Recursion* and the *Multistep left recursion*. For example, consider the following description: 
$$
A \rarr A \beta | \epsilon
$$
where A is a non-terminal term and $\beta$ is another production rule. For this case, when we are trying to substitute $A$, there will be an infinite recursion. This kind of recursion happens within one step of substitution and is therefore recognized as immediate left recursion. To eliminate this infinite recursion, we can use elimination rules to make 
$$
A \rarr A'
\\
A' \rarr \beta | \epsilon
$$
With this substituion, we can eliminate the infinite recursion problem when we are using the recursive descend algorithm. However, there is also another type of left recursion. Consider 
$$
S \rarr Aa|b
\\
A\rarr Ac|Sd|\epsilon
$$
In this case, by substitute two steps, we will have an implicit left recursion because $S\rarr Aa \rarr Sda$. To eliminate this kind of recursion, we need to apply the algorithm: 

<img src="https://imgge.com/images/2021/04/13/7f82058b15a872d8d9eebb3ec1b70ecf.png" alt="7f82058b15a872d8d9eebb3ec1b70ecf.png" style="zoom:50%;" />

##### 2.2 Remove Left Common Factor

After that, we can have a CFG that is without the worry of left recursion. Instead, we need to focus on the (1) property in the LL(1) grammar. If we want to choose the proper production rule to use with just looking ahead one token, we need to eliminate the common left factors. Here is an example of the common left factors in these two productions:
$$
A \rarr cf|\epsilon
\\
B \rarr cfgh
$$
Then, we have common left factor cf. With the lookahead token being $c$, there is no way we can identify which production rule we need to use. We need to make the productions:
$$
A \rarr cf A'
\\
A'\rarr gh | \epsilon
$$
The general algorithm that describes this procedure is: 

<img src="https://imgge.com/images/2021/04/14/1d3c0504b0d081d6fcdeae29c35ee036.png" alt="1d3c0504b0d081d6fcdeae29c35ee036.png" style="zoom:50%;" />

##### 2.3 Make First and Follow

After we have finish converting the grammar production rules into the LL(1) form, we can apply the general top-down parsing procedure. In the LL(1) implementation, we should use the recursive descend parsing to solve the problem. In order to know which production to take at each step, we need to calculate the first set and the follow set for each terminal or nonterminal symbol.  

In theory, the first set and the follow set are calculated like this:

<img src="https://imgge.com/images/2021/04/14/1f8ac7679ec64153305ad9bdab39869c.png" alt="1f8ac7679ec64153305ad9bdab39869c.png" style="zoom:67%;" />

<img src="https://imgge.com/images/2021/04/14/42caad723d186b2aa08ff58687ecd25e.png" alt="42caad723d186b2aa08ff58687ecd25e.png" style="zoom:67%;" />

Based on the first set and the follow set, we can construct a LL prediction table. For my implementation, I have directly constructed a tree denoting the CFG based on the first and follow set:

<img src="https://imgge.com/images/2021/04/14/4cb03725b7f410a41a20f5bcc5ca011c.png" alt="4cb03725b7f410a41a20f5bcc5ca011c.png" style="zoom:67%;" />

##### 2.4 Ambiguity in the grammar given

However, if we look closer to the grammar given, we can see that we have ignored the precedence between the different operators. For example, * should have a higher priority than + in calculation. Therefore, we need to apply another transformation on the productions rules associated with `exp`. 

<img src="https://imgge.com/images/2021/04/14/28310796a2eb2ee51d00b41acf1329ff.png" alt="28310796a2eb2ee51d00b41acf1329ff.png" style="zoom:50%;" />

Moreover, there is a **dangling else problem** in this grammar. A dangling else refers to this kind of situation:

![c3717d2ef2993a0abc6a1718a0b40384.png](https://imgge.com/images/2021/04/14/c3717d2ef2993a0abc6a1718a0b40384.png)

<img src="https://imgge.com/images/2021/04/14/e24ad15a20e243c777dcfa0729c4befa.png" alt="e24ad15a20e243c777dcfa0729c4befa.png" style="zoom: 50%;" />

Thus, in the implementation, I have chosen the conventional way in C, just to match the else with the nearest if.

##### 2.5 Program Design

In this program, I have created an abstract syntax node to maintain the AST structure. The production rules have inherited this node and implemented different parsing sub-routines based on the production rules. The header definition is like follows:

```c++
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
```

The detailed implementation of the classes can be found in the code.

### 3. Demo & Testings

The tree view of the directory is shown here:

![55af96bb99f443002ca5a50fc113f682.png](https://imgge.com/images/2021/04/14/55af96bb99f443002ca5a50fc113f682.png)

This is tested under the environment:

OS: MacOS Catalina 10.15.5

Compiler: Apple clang version 12.0.0 (clang-1200.0.32.29)
				  Target: x86_64-apple-darwin19.5.0
				  Thread model: posix

The program can be compiled using the CMake file provided:

```shell
mkdir build
cd build
cmake ..
```

If you do not have a cmake installed, I have included the build files in the folder. Just make using the makefile.

The sample output is shown here:

![8eeac13013fd0c31a99a8ae700a03c23.png](https://imgge.com/images/2021/04/14/8eeac13013fd0c31a99a8ae700a03c23.png)

### 4. Analysis

Although in this parser, I have got most of the grammar analysis done, (e.g. symbol precedence, production rules), the parser here is still missing something. Because I have been taking the LL(1) parsing technique, the operators are now all right associative. This was determined by the nature of recursive descend parsing routines. In order to allow the right association rules to be applied, we need to spin the grammar tree. This will be covered by the future work.

### 5. Reflections

What I have learned through this project is how to write a parser without using a parser generator. Moreover, the most important thing I have learn is the trade-off between LL parser and LR parser. LL parser is certainly easier to implement compared with the LR parser, but it is more restrictive in terms of the CFG. In order to use the LL parser, CFGs need to be pre-processed. However, as the CFGs only needs to be prepocessed and can be done. before runtime, it won't sacrific much performance.



