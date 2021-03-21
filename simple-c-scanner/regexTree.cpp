//
// Created by ccjeff on 2021/3/17.
//

#include "regexTree.h"

#include <iostream>
#include <cstddef>
#include <memory>
#include <string_view>
#include <unordered_set>

regexTree::Node::~Node() = default;

std::unique_ptr<regexTree::Node> regexTree::buildTree(std::string_view regex, bool star) {
    // if the regex is only for matching a specific char, firstpos and lastpos = 0
    if (regex.length() == 1 || (regex.front() == '\\' && regex.length() == 2)){
        auto leaf = std::make_unique<LeafNode>(leaves.size(),regex.length() == 1 ? regex[0] : regex[1]);
        this->leaves.emplace_back(leaf.get());
        return leaf;
    }

    int openParens = 0;
    for (std::size_t i = 0; i < regex.length(); ++i){
        //neglecting \( and \)
        if (regex[i] == '\\'){
            ++i;
            continue;
        }
        openParens += regex[i] == '(';
        openParens -= regex[i] == ')';
        if (regex[i] == '|' && openParens == 0){
            //subtree construction ..|..
            return std::make_unique<UnionNode>(buildTree(regex.substr(0,1)), buildTree(regex.substr(i+1)));
        }

    }
    const char lastChar = regex.back();
    switch (lastChar){
        case '*': {
            return buildTree(regex.substr(0,regex.length()-1), true);
        }
        case ')':{
            // locate where the close paren (single constructing block is)
            const auto open_paren_idx = [&] {
                int close_parens = 1;
                for (std::size_t i = regex.length() - 2; i >= 0; --i) {
                    close_parens += regex[i] == ')';
                    close_parens -= regex[i] == '(';
                    if (close_parens == 0) return i;
                }
                // unbalanced parens
                throw std::exception();
            }();
            if (open_paren_idx == 0) {
                if (star){
                    return std::make_unique<StarNode>(buildTree(regex.substr(1,regex.length()-2)));
                } else {
                    return buildTree(regex.substr(1,regex.length()-2));
                }
            } else {
                auto left = buildTree(regex.substr(0,open_paren_idx));
                auto right = [&]() -> std::unique_ptr<Node> {
                    if (star) {
                        return std::make_unique<StarNode>(buildTree(regex.substr(open_paren_idx)));
                    } else {
                        return buildTree(regex.substr(open_paren_idx));
                    }
                }();
            }
        }
        default: {
            auto left = buildTree(regex.substr(0,regex.length()-1));
            auto right = [&]() -> std::unique_ptr<Node> {
                if (star) {
                    return std::make_unique<StarNode>(
                            buildTree(regex.substr(regex.length() - 1)));
                } else {
                    return buildTree(regex.substr(regex.length() - 1));
                }
            }();
            return std::make_unique<CatNode>(std::move(left), std::move(right));
        }
    }
}

std::unordered_set<char> regexTree::Alphabet(regexTree::Node* n) {
    if (auto node = dynamic_cast<regexTree::CatNode*>(n)) {
        auto alphabet1 = Alphabet(node->left.get());
        auto alphabet2 = Alphabet(node->right.get());
        alphabet1.insert(alphabet2.begin(), alphabet2.end());
        return alphabet1;
    } else if (auto node = dynamic_cast<regexTree::UnionNode*>(n)) {
        auto alphabet1 = Alphabet(node->left.get());
        auto alphabet2 = Alphabet(node->right.get());
        alphabet1.insert(alphabet2.begin(), alphabet2.end());
        return alphabet1;
    } else if (auto node = dynamic_cast<regexTree::StarNode*>(n)) {
        return Alphabet(node->child.get());
    } else if (auto node = dynamic_cast<regexTree::LeafNode*>(n)) {
        return std::unordered_set<char>({node->label});
    } else if (auto node = dynamic_cast<regexTree::EndNode*>(n)) {
        return std::unordered_set<char>();
    } else {
        // there is no 6th type of RegexTree nodes
        throw std::exception();
    }
}

std::unique_ptr<regexTree::Node> regexTree::catEndNode(std::unique_ptr<regexTree::Node> root) {
    auto endNode = std::make_unique<EndNode>(endPos());
    return std::make_unique<CatNode>(std::move(root), std::move(endNode));
}

void regexTree::calcFollowPos(regexTree::Node *n) {
    if (auto node = dynamic_cast<regexTree::UnionNode*>(n)) {
        calcFollowPos(node->left.get());
        calcFollowPos(node->right.get());
    } else if (auto node = dynamic_cast<regexTree::CatNode*>(n)) {
        for (auto i : node->left->lastpos) {
            leaves[i]->followpos.insert(node->right->firstpos.cbegin(),
                                        node->right->firstpos.cend());
        }
        calcFollowPos(node->left.get());
        calcFollowPos(node->right.get());
    } else if (auto node = dynamic_cast<regexTree::StarNode*>(n)) {
        for (auto i : node->lastpos) {
            leaves[i]->followpos.insert(node->firstpos.cbegin(),
                                        node->firstpos.cend());
        }
        calcFollowPos(node->child.get());
    }
}