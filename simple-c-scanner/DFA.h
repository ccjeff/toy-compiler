//
// Created by ccjeff on 2021/3/19.
//
#ifndef SCANNER_DFA_H
#define SCANNER_DFA_H

#include "regexTree.h"
#include <string_view>
#include <vector>

class DFA {
public:
    class State {
    private:
        struct Transition {
            Transition(const State* s, char c) : state(s), character(c) {}
            const State* state;
            char character;
        };

    public:
        void AddTransition(State* state, char c) {
            transitions.emplace_back(state, c);
        }
        bool IsAcceptState() const { return is_accept_state; }
        void MakeAcceptState() { is_accept_state = true; }
        const std::vector<Transition>& Transitions() const { return transitions; }
    private:
        std::vector<Transition> transitions;
        bool is_accept_state; //terminal state for the DFA
    };

    explicit DFA(const RegexTree& tree);
    // simulates the DFA with the given string sentence, true if the sentence match the DFA pattern
    bool simulateDFA(std::string_view sentence);

    // can be used to debug for the program
    void CreateDotFile(std::string_view filename) const;
private:
    std::vector<std::unique_ptr<State>> states;
};


#endif //SCANNER_DFA_H
