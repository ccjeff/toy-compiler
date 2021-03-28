//
// Created by ccjeff on 2021/3/17.
//
#include "regexTree.h"

#include <cstddef>
#include <memory>
#include <string_view>
#include <unordered_set>

RegexTree::Node::~Node() {}

std::unique_ptr<RegexTree::Node> RegexTree::buildTree(std::string_view regex,
                                                      bool star) {
    // dealing with single word regex, used for reserved words and symbols
    if (regex.length() == 1 || (regex.front() == '\\' && regex.length() == 2)) {
        auto leaf = std::make_unique<LeafNode>(
                leaves.size(), regex.length() == 1 ? regex[0] : regex[1]);
        leaves.emplace_back(leaf.get());
        return leaf;
    }

    // match regex of the type: ...|...
    int open_parens = 0;
    for (std::size_t i = 0; i < regex.length(); ++i) {
        // don't consider "\(" or "\)" as regex parens
        if (regex[i] == '\\') {
            ++i;
            continue;
        }
        open_parens += regex[i] == '(';
        open_parens -= regex[i] == ')';
        // for the "||" token only, return a root with | on each side
        if (regex == "||"){
            return std::make_unique<UnionNode>(buildTree("|"), buildTree("|"));
        }
        // call recursive construction
        if (regex[i] == '|' && open_parens == 0) {
            return std::make_unique<UnionNode>(buildTree(regex.substr(0, i)),buildTree(regex.substr(i + 1)));
        }
    }

    switch (regex.back()) {
        case '*': {
            return buildTree(regex.substr(0, regex.length() - 1), true);
        }
        case ')': {
            const auto openParanNumber = [&] {
                int close_parens = 1;
                for (std::size_t i = regex.length() - 2; i >= 0; --i) {
                    close_parens += regex[i] == ')';
                    close_parens -= regex[i] == '(';
                    if (close_parens == 0) return i;
                }
                // unbalanced parens
                throw std::exception();
            }();

            // (...)
            if (openParanNumber == 0) {
                if (star) {
                    return std::make_unique<StarNode>(
                            buildTree(regex.substr(1, regex.length() - 2)));
                } else {
                    return buildTree(regex.substr(1, regex.length() - 2));
                }
            }
                // ...(...)
            else {
                auto left = buildTree(regex.substr(0, openParanNumber));
                auto right = [&]() -> std::unique_ptr<Node> {
                    if (star) {
                        return std::make_unique<StarNode>(
                                buildTree(regex.substr(openParanNumber)));
                    } else {
                        return buildTree(regex.substr(openParanNumber));
                    }
                }();
                return std::make_unique<ConcatNode>(std::move(left), std::move(right));
            }
        }
        default: {
            // ...
            auto left = buildTree(regex.substr(0, regex.length() - 1));
            auto right = [&]() -> std::unique_ptr<Node> {
                if (star) {
                    return std::make_unique<StarNode>(
                            buildTree(regex.substr(regex.length() - 1)));
                } else {
                    return buildTree(regex.substr(regex.length() - 1));
                }
            }();
            return std::make_unique<ConcatNode>(std::move(left), std::move(right));
        }
    }
}

std::unordered_set<char> RegexTree::getAlphabets(RegexTree::Node* n) {
    if (auto node = dynamic_cast<RegexTree::ConcatNode*>(n)) {
        auto alphabet1 = getAlphabets(node->left.get());
        auto alphabet2 = getAlphabets(node->right.get());
        alphabet1.insert(alphabet2.begin(), alphabet2.end());
        return alphabet1;
    } else if (auto node = dynamic_cast<RegexTree::UnionNode*>(n)) {
        auto alphabet1 = getAlphabets(node->left.get());
        auto alphabet2 = getAlphabets(node->right.get());
        alphabet1.insert(alphabet2.begin(), alphabet2.end());
        return alphabet1;
    } else if (auto node = dynamic_cast<RegexTree::StarNode*>(n)) {
        return getAlphabets(node->child.get());
    } else if (auto node = dynamic_cast<RegexTree::LeafNode*>(n)) {
        return std::unordered_set<char>({node->label});
    } else if (auto node = dynamic_cast<RegexTree::EndNode*>(n)) {
        return std::unordered_set<char>();
    } else {
        throw std::exception();
    }
}

std::unique_ptr<RegexTree::Node> RegexTree::ConcatEndNode(
        std::unique_ptr<RegexTree::Node> root) {
    auto end_node = std::make_unique<EndNode>(endpos());
    return std::make_unique<ConcatNode>(std::move(root), std::move(end_node));
}

void RegexTree::getFollowpos(RegexTree::Node* n) {
    if (auto node = dynamic_cast<RegexTree::UnionNode*>(n)) {
        getFollowpos(node->left.get());
        getFollowpos(node->right.get());
    } else if (auto node = dynamic_cast<RegexTree::ConcatNode*>(n)) {
        for (auto i : node->left->lastpos) {
            leaves[i]->followpos.insert(node->right->firstpos.cbegin(),node->right->firstpos.cend());
        }
        getFollowpos(node->left.get());
        getFollowpos(node->right.get());
    } else if (auto node = dynamic_cast<RegexTree::StarNode*>(n)) {
        for (auto i : node->lastpos) {
            leaves[i]->followpos.insert(node->firstpos.cbegin(),node->firstpos.cend());
        }
        getFollowpos(node->child.get());
    }
}
