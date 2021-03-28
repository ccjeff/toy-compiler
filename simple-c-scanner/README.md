## CSC4180 Assignment 2 Simple C Scanner

117010008 曹正家

### 1. Introduction

In this assignment, we are implementing a scanner for the simplified C language. The scanner is the very first constructing part of the front-end of the compiler that is responsible of tokenizing the input data stream and return the different tokens and their associating types for further analysis. For example, the `int a = 10;` should be returing a stream of: `int` token, `tokenID = 0`, `ID`token, `tokenID = 1`, `=` token, `tokenID = 2`, `DIGIT`token with ID 3, and `SEMI` token with ID 4. The complete output pattern and the token list will be shown in the later sections.

The scanner can be further decomposed into three different components: the regex tree constructor, the DFA composer and the scanner controller (including the FileIO part). The following tree view shows the structure of the repository.

```shell
.
├── CMakeLists.txt
├── CSC4180\ Assignment\ 2\ Simple\ C\ Scanner.md
├── DFA.cpp
├── DFA.h
├── fileIO.cpp
├── fileIO.h
├── regexTree.cpp
├── regexTree.h
├── scanner.cpp
├── scanner.h
└── test_files
    ├── test_easy.in
    └── test_hard.in
```

The FileIO class is used to load the content of the file to scan into buffer in the memory. The regexTree class will take in pre-defined regular expression definitions for different tokens. It is directly used in the DFA class, which takes the RegexTree structure as input and outputs a deterministic finite automata that is a deterministic representation of the regular expression rule. Each regular expressions are translated independently and combined together as the grammar definition for this language. 

The design of the scanner is following the structure of the Flex program demonstrated in the dragon book (compiler principles) with some modifications.

<img src="https://imgge.com/images/2021/03/28/1f6b2959d4c9d8b922d41a0ff58f9917.png" alt="1f6b2959d4c9d8b922d41a0ff58f9917.png" style="zoom:33%;" />

### 2. System Design

##### 2.1 General design 

As have previously showned, the program is consisted of three major components. To be more specific, the  structure of the my implementation can be shown in the following flow diagram:

<img src="https://imgge.com/images/2021/03/28/3ba2cb3406dfc9b6d106606d8101f796.png" alt="3ba2cb3406dfc9b6d106606d8101f796.png" style="zoom:50%;" />

Where in this diagram, a rectangle denotes an instance, and a rounded rectangle means a method called. We can see that the program started with taking file inputs, save it to buffer and wait for the DFAs to read the corresponding string to match. Note that these to two parts have no direct data dependency, the DFA constructions can be done with the FileIO in parallel. Moreover, the multiple DFAs also gives room for parallel speedup. Due to the time constraints, I have not implemented this part. I would argue that the best solution would be creating these seperate DFAs concurrently using pthreads, and combine them into a NFA, then apply the NFA to DFA algorithm to create a universal DFA. This will be implemented in the future.

##### 2.2 RegexTree

The regexTree is the first components constructed to define the grammar rules of the simplified C language. The RegexTree class is defined as a tree structure linked by private Node classes. Since there are many different types of operations defined in the regular expression, we have to created different Nodes to allow their functions. This is implemented with a general Node class and inhertied Node classes overriding its methods.To be specific, in the regular expression applied here, I have defined four different Node Classes, the `ConcatNode`, the `UnionNode`, the `StarNode`, the `LeafNode` and the `EndNode` denoting `·(concat), |(union),*(star),#(end)`operators. Note that the `#` operator is used in the extended regular expression grammar as the end state. This is used because when using the direct transition from regex to DFA, we need to care about the **important states**. We call the state of a NFA important if it has a non-$\epsilon$ out transition. Since the final state is not important, we need a symbol to define it in the regex to make it important. The detail of this algorithm will be analyzed in the DFA section. The important states of the NFA correspond directly to the positions in the regular expression that hold symbols of the alphabet.

The reason why we only create these nodes here is simple: the regex notation of `a+` and is replace by `(a)|(a)*` and the `(a|b)?c+` is replace by `(a|b|c)c.` This does not change the definition of the regex but also ensures that the symbol `+` and`?` can be treated just as normal inputs and avoid confusion with the `PLUSOP`.

Therefore, we can construct a regular expression (e.g. `(a|b)*abb#`) into the form of tree as demonstrated in the next picture:

<img src="https://imgge.com/images/2021/03/28/cbaeb6cedcac47b8f2f347276bd6a355.png" alt="cbaeb6cedcac47b8f2f347276bd6a355.png" style="zoom:33%;" />

Along constructing the tree, these values are also calculated: `followpos`, `nullable`, `lastpos` and `firstpos`. These values are discussed in the next section. The tree construction is based on the DFS approach using recursion. I have categorized the regex into these different types:

1. In form ...|...

   For regex of this form, the regex tree is constructed by calling recursive constructions on left and right substring of the | symbol. The UnionNode will become the root of the tree.

2. In form ...*  (* symbol at the end of the regex)

   For regex with and ending star, then it marks the star node as true in the tree constructor and build the tree using recursive call on the previous substring.

3. In form (...)

   The program will remove the parens and call constructor based on ...

4. In form ...(...)

   build tree using catnode as the root, with left subtree calling constructor for ... and right subtree by calling constructor for (...)

5. in form ...

   build a right leaning tree, each time reads one character as the left node. Connected using the catnode and call constructor for the rest of the string

 ##### 2.3 DFA construction

In this seciton, we will be discussing further details of the DFA structure and the construcion algorithm. Before diving into the details of the algorithm, we are to define the variables first:

`nullable(n)` is true for a syntax-tree node n if and only if the subexpression represented by n has $\epsilon$ in its language (eps operation in the NFA). That is, the subexpressiort can be "made null" or the empty string, even though there may be other strings it can represent as well.

`firstpos(n)` is the set of positions in the subtree rooted at n that correspond to the first symbol of at least one string in the language of the subexpression rooted at n. It is the set containing all the possible characters of being the first symbol of the string.

`lastpos(n)` is the set of positions in the subtree rooted at n that correspond to the last symbol of at least one string in the language of the subexpression rooted at n. Similar to the firstpos, it is the set containing symbols that can be the last symbol of the string.

`followpos(n)` is the set of positions in the entire syntax tree such that there is some string $x = a_1a_2...a_n$ in $L(r\#)$ such that for some i, there is a way to explain the membership of x in $L(r\#)$ by matching $a_i$ to the position p of the syntax tree and $a_i+1$ to position q.

We can be using the algorithm as concluded in the following picture:

<img src="https://imgge.com/images/2021/03/28/9969b90fe3c74f910305acde08a62ab2.png" alt="9969b90fe3c74f910305acde08a62ab2.png" style="zoom:33%;" />

Moreover, the `followpos` follows the idea that there are only two ways that a position of a regular expression can be made to follow another:

1. If n is a cat-node with left child $c_1$ and right child $c_2$ , then for every position i in `lastpos(c1)`, all positions in firstpos(c2) are in followpos(i).
2. If n is a star-node and i is a position in lastpos(n), then all position in firstpos(n) are in followpos(n).

The intuition in this algorithm of directly translating regular expressions to DFAs is that when we are using subset construction algorithm to construct the DFA from a NFA, we are using only the important states in a set T when it computes the $\epsilon$ - closure(move(T,a)). That is, the set of states move(s,a) is non-empty only when state s is important. Therefore, we can be caring only about the important states and convert NFA to DFA along the NFA construction.

The DFA class is implemented as a linked structure of State instances. Each State instance contains a Transition structure, which will contain the pointer to the ending state, as well as the transition character. Using the linked structure, we can implement the simulation of the DFA to determine if a given string belongs to the token type that the DFA denotes.

##### 2.4 Precedence definition

In some cases, a token can match two token regexs. For example, the when reading the word "main", it matches the token "MAIN" and the token "ID" at the same time. If this happens, this was avoided using the pre-defined precedence list. The precedence list makes sure the DFA placed at the beginning of the vector initialization with a higher priority. The complete vector initialization is shown here:

```c++
allRules = {intTokenDFA, voidTokenDFA, breakTokenDFA, ifTokenDFA, elseTokenDFA, doTokenDFA,
                mainTokenDFA, printfTokenDFA,scanfTokenDFA, returnTokenDFA, whileTokenDFA,
                equalTokenDFA,semicolonDFA, addDFA, minDFA,multiDFA, divDFA, andDFA, andandDFA, orDFA, commaDFA,
                rbracDFA, lbracDFA, rsquareDFA, lsquareDFA,lparenDFA, rparenDFA,notDFA, 		rtDFA,ltDFA,shiftLDFA,shiftRDFA, equTokenDFA, neqTokenDFA, lessEquDFA, greatEquDFA,digitDFA,idDFA};
```

For the case of "maina", we have the situation that before reading the character a, there are multiple matched DFAs. This is done by using the following method:

```c++
std::string matchHelper(std::string* input,DFA* rule,FileIO* src){
    int counter = 1;
    bool firstMatch = false;
    while (counter != 0){
        counter = 0;
        bool isMatch = rule->simulateDFA(*input);
        counter += isMatch;
        if (isMatch) {firstMatch = true; input->push_back(src->getNextChar());}
    }
    if (firstMatch){
        input->pop_back();
        src->retractLex();
    }
    if (!firstMatch) return "";
    return *input;
}

int fetchToken(std::string* inputs, FileIO* src){
    // each round getting a string and return a type of token
    // first go find the matched dfa and read until it is fucked
    auto allRules = getRules();
    std::string currentWord = "";
    int count = 0;
    int pos = -1;
    for (auto rule : allRules){
        std::string filename = std::to_string(count);
        filename += "_rule.gv";
        rule->CreateDotFile(filename);
        auto res = matchHelper(inputs, rule, src);
        if (res.size() > currentWord.size()) {currentWord = res; pos = count;}
        count++;
    }
    std::cout << "TOKEN NAME: " << *inputs << std::endl;
    return pos;
}
```

This will make sure at each round, each DFA reads as much matching characters as possible and return it to the fetchToken function. We will take the DFA with the longest matched string as the token type of this word.

### 3. Demo & Testings

I have tested my program with two files `test_easy.in` and `test_hard.in` under the `test_files` directory. The test output is shown in the screenshot:

<img src="https://imgge.com/images/2021/03/29/a5e86c5ee8640aeda146c75c966aa7d8.png" alt="a5e86c5ee8640aeda146c75c966aa7d8.png" style="zoom:50%;" />

This is tested under the environment:

OS: MacOS Catalina 10.15.5

Compiler: Apple clang version 12.0.0 (clang-1200.0.32.29)
				  Target: x86_64-apple-darwin19.5.0
				  Thread model: posix

The program can be compiled using the CMake file provided:

<img src="https://imgge.com/images/2021/03/29/0e6769ca6b7c72519d542f8023bb7baf.png" alt="0e6769ca6b7c72519d542f8023bb7baf.png" style="zoom:50%;" />

just by typing:

```shell
cd build
make
./scanner ../test_files/test_hard.in
```

### 4. Reflections

What I have learned through this project is how Flex is constructed using the DFA. Although I might not be taking the most optimized approach for compile time, I believe my method have catched the essence of the project. It have been using regex tree for regular expression analysis, and have covered the DFA conversion algorithm. Moreover, I have tested my program so that it otuputs the correct token id. I believe my method is generalizable to all grammar rules that can be defined by regular expressions and can save a lot of future work than using the brute force matching approach using the `if...else` control flow.

If time allows, I will implement the further conversion of parallel conversion of DFAs, connecting these DFAs into a NFA and run the DFA conversion again. I believe this can further optimize the scanner runtime.

(P.S. 龙书的中文版翻译太坑了TAT)



Reference:

1. Compiler: Principles, Techniques and Tools. by Alfred V. Aho, Monica S. Lam, Ravi Sethi, and Jeffrey D. Ullman.
2. https://changkun.de/modern-cpp/zh-cn/05-pointers/index.html Modern C++ guide
3. https://github.com/ghallak/regex-to-dfa Ghallak implementation of regex to DFA algorithm, used for generate DFA graphs for debugging
4. http://www.cs.cornell.edu/courses/cs312/2007fa/recitations/rec26.html Regex and DFA 