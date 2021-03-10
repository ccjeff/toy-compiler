### Project 1: Micro Compiler

### 1. Purpose

The micro language is a toy language with very simple grammar rules. In fact, the language functions just like a toy calculator with simple add,subtract, read and write operations. The purpose of this project is to implement a compiler for micro language in order to get a better understanding of the compiling process. The input file is a file written in micro language, and the expected output of this compiler is a .asm file in MIPS assebly. The .asm file is later passed into the MIPS simulator to execute the MIPS code. The .asm is written in a customized fashion (following the MIPS ISA but no external declarations) thus cannot be executed without the MIPS simulator provided in this repository.

Before jumping into the design of the compiler, I will list out the CFG for the language and provide a sample format for the input.

<img src="https://imgge.com/images/2021/03/09/110a4a4c8331f699050663065ff080c7.png" alt="110a4a4c8331f699050663065ff080c7.png" style="zoom: 20%;" />

However, there are some minor change to the above stated CFG. The `begin` and `end` statement in the CFG for this compiler should all be CAPITALIZED. The compiler is case sensitive for these two tokens and would generate a compile error when being fed with lower case `begin` and `end`. Also, the `SCANEOF` token which represents the end of the file was defined as the `$` symbol. Missing the $ sign at the end of your input file would also generate an error.

Here is a sample input file:

```micro
BEGIN
READ(A);
A := (2+A)+1+12;
c := A + 1;
d := 111111;
WRITE(A,c,A+c);
END$
```

The code will be translated into 

```assembly
move $sp,$fp
addi $sp,$sp,4
sw $zero,4($fp)
li $v0,5
syscall
move $t1,$v0
sw $t1,4($fp)
addi $sp,$sp,4

.
.
.
.

add $t2,$t1,$t2
lw $t3,24($fp)
add $t3,$t2,$t3
sw $t3,32($fp)
lw $t1,32($fp)
li $v0,1
move $a0,$t1
syscall

```

The specific process of this compilation will be covered in the next section.

### 2. Method and Design

#### 2.1 General Design

Just like most of the modern compilers, the micro compiler for this project can be divided into two parts: the front-end and the back-end. The front-end consists of the **scanner** and the **parser**. Whereas the backend of the compiler takes care of the code-generation part. In this project, I have used the flex to implement the scanner and the bison yacc to implement the parser. The workflow diagram is shown here:

<img src="https://imgge.com/images/2021/03/09/194baca1c9be1681d6e6a0e7663b7995.png" alt="194baca1c9be1681d6e6a0e7663b7995.png" style="zoom:50%;" />

By the time the input is passed through the parser, a intermediate code is generated. The intermediate code I have used for this project is the standard 3 address code. Although there is no optimization on register allocation involved in this project, the 3 address code allows easy convertion to MIPS assembly. The intermediate code is generated and stored in a `a.txt` file under the compilation folder. You can check the 3 address code for a better understanding of the compilation process of the program than the final assembly code (This is just so hard to read). However, this also means that there should not be an exisiting file named a.txt within the comiler folder.

#### 2.2 Scanner

The scanner is considered to be the very first component of the compiler because every file is first passed to the scanner to be tokenized. This can help us extract different words that made up the statements in the input file. The extraction of the tokens are done by applying regular expression matching to the current line. Specifically, the token and the extracting regex statements are defined as follows in the `scanner.l` file.

```c
:=              return ASSIGNOP;
,               return COMMA;
\+               return PLUOP;
--.*\n                  ;
-               return MINUSOP;
;               return SEMICOLON;
\(               return LPAREN;
\)               return RPAREN;
\$               return SCANEOF;

"BEGIN"         return BEGINN;
"END"           return END;
"READ"          return READ;
"WRITE"         return WRITE;

[a-zA-Z][-_a-zA-Z0-9]*	{yylval.id = strdup(yytext); return ID;}
[1-9][0-9]*				{yylval.num = atoi(yytext); return INTLITERAL;}
[ \t\n]                 ;
.					printf("unexpected character\n");
```

 As stated above, the scanner will apply regex to find matching characters and return a pre-defined token ID to represent the characters. The specific token definitions was automatically done by bison in the `y.tab.h` file. If you want to use the scanner alone, the definition of tokens can also be found in the `./test_utils/scanner.h`. Replace the `y.tab.h` in the including header of the `scanner.c` and use the make file to compile.

Here is a screenshot of the scanner test alone:

<img src="https://imgge.com/images/2021/03/10/53df90f142a2a5645fa8ed03de64ccf1.png" alt="53df90f142a2a5645fa8ed03de64ccf1.png" style="zoom: 20%;" />

The input file is successfully read, and the tokens in the file are all successfully recognized. The variable name and the integer value is passed to the parser for further analysis through return statement in the scanner using `yytext`. The details of the implementation can be found inside the code. 

#### 2.3 Parser

The parser is done by using bison, a GNU compiler compiler that is compatible with yacc. The job of the parser is to generate a three address code as the intermediate representation for future processing such as register optimization and code generation. For our project, the parser is not implemented using the syntax tree. Unlike the usual schema, the parsing routine was implemented through recursive parsing and the information is stored using a `expr_record` structure that was constructed for each statement or expression.

The most interesting parsing problem in the parser is *the multiple add* problem. In the micro grammar, we can have a statement like `a := (1-b) + (2+3-b) + 3`, and in this problem, the expression at the right hand side of the assignment symbol will be treated as a single expression. The parser will continue calling the parsing routine for the expression in the next step. The parser shall quickly realize that the expression should be recognized as a expression add a primary, and calling the corresponding routine until the recursive calls finds a base case where the parser recognize an ID (char*) or a integer. Along the way, the temporary expressions are all stored. In the process of tranlating to the three address code, these expressions are solved using a temporary variable declared.

The `expr_record` structure described above was implemented as the following in file  `translate.h`:

```C
enum expr { IDEXPR, LITERALEXPR, TEMPEXPR };

typedef struct expression{
	enum expr kind;
	string name;  /* for IDEXPR, TEMPEXPR */
	int val;	     /* for LITERALEXPR */
} expr_rec;
```

 The output 3 address code is shown like this:

```assembly
Declare,A,Integer
Read,A
Declare,Temp&1,Integer
Add,Temp&1,2,A
Declare,Temp&2,Integer
Add,Temp&2,Temp&1,1
.
.
.
Declare,Temp&5,Integer
Add,Temp&5,A,c
Write,Temp&5
Halt
```

Thus, you can see that a temp variable is almost always associated with the add/subtract operation. A minor issue with the 3 address code outputed by the parser is that when it comes to adding or subtracting two temporary variables, the order is not strictly following the traditional `Add destination, source1, source2` format. It was `Add source1, source2, destination`for debugging ease. This is then fixed by the translator in the next code generation stage for generating MIPS code.

The action symbols for each grammar rule is the same as stated on the slides. For more reference, please go to the comments inside the source code.

<img src="https://imgge.com/images/2021/03/10/dcf091637bd48f1fa6473cf627f5af15.png" alt="dcf091637bd48f1fa6473cf627f5af15.png" style="zoom:100%;" />

#### 2.4 Translator

By the time the file has been passed into the translator, it was already been checked for grammar correctness and converted into three address code. Then the three address code is then passed to the backend, which is the `MIPS_translate.py` file. This will convert the 3 address code into the MIPS code. Direct switch translate was used in this file.

```python
class Translator:
  def __init__(self, filename):
  def readFile(self):
  def translation(self,line):
 	def translationRoutine(self):
        self.readFile()
        print("move $sp,$fp")   #initializing stack pointer $29 to 0
        for i in self.threeAddrs:
            self.translation(i)
```

The `$fp` value was initialized to `$zero` in the translator because we do not have a real memory address available. Therefore, all the memory address used is the fp-related address. Each time we declare a variable, the stack pointer will move above for 4 byte to store this integer value. The memory layout can be shown with this picture:

<img src="https://imgge.com/images/2021/03/10/57124e4fd563132c79f303c085c38529.png" alt="57124e4fd563132c79f303c085c38529.png" style="zoom:15%;" />

The rest of the translation can be done based on the tokens. Based on different variable numbers, I have further classified them into two category: 

length-3 op: `Declare, Read, Write, ...` 

Length-4 op: `Add, Sub`

### 3. Experiment Environment

OS: MacOS Catalina 10.15.5

Compiler: Apple clang version 12.0.0 (clang-1200.0.32.29)
				  Target: x86_64-apple-darwin19.5.0
				  Thread model: posix

Flex: flex 2.5.35 Apple(flex-32)

Bison: bison (GNU Bison) 2.3

**Execution Procedure:**

First, specify the input file name in the `test.sh` file. 

```shell
./test.sh

#### the content in ./test.sh is:
# if the shell file does not work, please use these commands

# #! /bin/sh
# make clean
# make micro
# ./micro.out ./testfiles/test_easy.in > a.txt
# python3 mips_translate.py > output.asm
# python3 mips_simulator.py
```

this will invoke the makefile to compile the testfile and run it through the MIPS simulator implemented in python. After the execution, you shall find the `a.txt` containing the content of the three address code and the `output.asm` file containing the translated MIPS assembly code.

The execution screenshot is like follows:

<img src="https://imgge.com/images/2021/03/10/d7f54d033c5e433a6d575f11744175ae.png" alt="d7f54d033c5e433a6d575f11744175ae.png" style="zoom: 20%;" />

In the experiment sections, I have provided some test cases, the corresponding execution results can be found in the later result and analysis section.

The test cases have considered many common corner cases. These files are stored under the ./testfiles/ folder. A common testcase file is like this:

```micro
BEGIN
READ(A);
A := (2+A)+1+12;
c := A + 1;
d := 111111;
WRITE(A,c,A+c);
END$
```

The testfile was also constructed for syntax error scenarios. The compiler can recognize the existence of syntax error and abort execution.

### 4. Result and Analysis

#### 4.1 Basic assignment

The testfile will try to assign values to different variables. In the simulator system I am building, the memory space can only hold 100 variables. The demo can be shown in the following graphs:

<img src="https://imgge.com/images/2021/03/10/f9028c7ed2d5ca2ed1bc1d6c757d8cc9.png" alt="f9028c7ed2d5ca2ed1bc1d6c757d8cc9.png" style="zoom:20%;" />

<img src="https://imgge.com/images/2021/03/10/77411be066dbd3a56b3561545aeba5b7.png" alt="77411be066dbd3a56b3561545aeba5b7.png" style="zoom:20%;" />

The first graph shows the normal execution result of the correct execution. However, when the code is demanding more memory than the system existence, it will generate an error. *The value of the memory can be changed by changing the defined value for symbol table capacity easily.*

#### 4.2 IO operations & Arithmetic operations

There are two IO operations existing in the micro language. The micro language use READ() to get user input of an integer value and assign it to a variable. 

<img src="https://imgge.com/images/2021/03/10/f4de5b47b62c77cfb373c839ea729089.png" alt="f4de5b47b62c77cfb373c839ea729089.png" style="zoom:20%;" />

#### 4.3 Analysis

The approach for this micro compiler is definitely not the general approach for designing all compilers. The grammar is simple and does not involve logical operations like the `for` and `if` statements. Therefore, I have been using the direct translation approach rather than using a syntax tree method. In this project, since the grammar is simple, direct translation into 3 address code might even be faster than the syntax tree method. The latter one should construct a tree first, and use the tree traversal to allow translation of input micro file into immediate 3 address code representation. 

However, using the syntax tree can have some other benefits. Think of the variable management when it comes to statements with `()`. In my approach, everytime a temporary variable is used. This is not only memory consuming but also time consuming in the later execution section. Since the number of registers is limited, each time using a temp variable might be causing a `lw-sw` routine. If we are dealing with real memory, this should slow down the program execution.

Luckily, we are using a simulator. The register and memory have no difference in terms of access speed.

### 5. Reflections

In this project, the fundamental workflow of the compiler has been shown. Although Micro is a really simple language used only as a toy, the project gives me the chance to see the essence of compiler. While the front-end of the compiler, the scanner and parser, is generated using flex and bison, the code generation routines and the backend of the compiler was constructed by myself. This is a great complement to the CSC3050 course. In the architecture course, we have learned how the MIPS code is translated into machine code and its execution in CPU, but there is a missing puzzle of how the high level languages, like Java, was translated to assembly codes. 





