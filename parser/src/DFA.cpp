//
// Created by ccjeff on 2021/3/19.
//
#include "DFA.h"
#include "regexTree.h"

#include <algorithm>
#include <cstddef>
#include <memory>
#include <string_view>
#include <utility>
#include <vector>


// a dstate is a state paired with its follow pos, this is needed when the dfa
// is being created. Globally declared.
struct DState {
    DState(std::unique_ptr<DFA::State> sp,
           const std::unordered_set<std::size_t>& fp)
            : state(std::move(sp)), followpos(fp) {}
    DState(std::unique_ptr<DFA::State> sp, std::unordered_set<std::size_t>&& fp)
            : state(std::move(sp)), followpos(std::move(fp)) {}

    std::unique_ptr<DFA::State> state;
    std::unordered_set<std::size_t> followpos;
};


DFA::DFA(const RegexTree& tree) {
    std::vector<DState> dstates;
    // add the initial state
    dstates.emplace_back(std::make_unique<State>(), tree.rootFirstpos());

    auto& alphabet = tree.Alphabet();

    for (std::size_t i = 0; i < dstates.size(); ++i) {
        // accepting states have the end of the regex in their followpos
        if (std::any_of(dstates[i].followpos.cbegin(), dstates[i].followpos.cend(),
                        [endpos = tree.endpos()](auto next_pos) {
                            return next_pos == endpos;
                        })) {
            dstates[i].state->MakeAcceptState();
        }

        for (auto character : alphabet) {
            std::unordered_set<std::size_t> new_next_positions;

            for (auto next_pos : dstates[i].followpos) {
                if (tree.charAt(character, next_pos)) {
                    auto followpos = tree.followpos(next_pos);
                    new_next_positions.insert(followpos.cbegin(), followpos.cend());
                }
            }

            if (new_next_positions.empty()) continue;

            auto old_dstate = std::find_if(
                    dstates.cbegin(), dstates.cend(),
                    [& nnp = std::as_const(new_next_positions)](const auto& dstate) {
                        return nnp == dstate.followpos;
                    });

            if (old_dstate == dstates.end()) {
                dstates.emplace_back(std::make_unique<State>(),
                                     std::move(new_next_positions));
                dstates[i].state->AddTransition(dstates.back().state.get(), character);
            } else {
                dstates[i].state->AddTransition(old_dstate->state.get(), character);
            }
        }
    }

    // move the states ownership to the DFA
    for (auto& dstate : dstates) states.emplace_back(std::move(dstate.state));
}


bool DFA::simulateDFA(std::string_view sentence){
    std::size_t matchCount = 0;
    auto p = states[0].get(); // init state
    while (!p->Transitions().empty()){
        auto nextStates = p->Transitions();
        auto action = std::find_if(nextStates.cbegin(), nextStates.cend(),
                                   [&nc = std::as_const(sentence[matchCount])](const auto& trans){
                                       return trans.character == nc;
                                   });
        if (action!=nextStates.end()){
            p = (State*)action->state;
            matchCount++;
            if (matchCount == sentence.size()) return true;
        } else {
            // stuck at some state
            return false;
        }
    }
    return false;
}


//void DFA::CreateDotFile(std::string_view filename) const {
//    std::ofstream out(filename.data());
//    out << "digraph finite_state_machine {" << '\n'
//        << "  rankdir=LR;" << '\n'
//        << "  size=\"18,15\"" << '\n'
//        << "  node [shape = circle]; q0" << '\n';
//
//    std::unordered_map<const State*, std::size_t> state_id;
//    for (std::size_t i = 0; i < states.size(); ++i) {
//        auto p = states[i].get();
//        for (const auto& trans : p->Transitions()) {
//            state_id.try_emplace(p, state_id.size());
//            state_id.try_emplace(trans.state, state_id.size());
//            auto start_state = "q" + std::to_string(state_id[p]);
//            auto end_state = "q" + std::to_string(state_id[trans.state]);
//            auto edge_label = trans.character;
//
//            if (trans.state->IsAcceptState()) {
//                out << "node [shape = doublecircle];" << '\n';
//            } else {
//                out << "node [shape = circle];" << '\n';
//            }
//            out << "  " << start_state << " -> " << end_state << " [ label = \""
//                << edge_label << "\" ];" << '\n';
//        }
//    }
//    out << "}" << '\n';
//}
