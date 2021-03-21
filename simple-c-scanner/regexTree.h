//
// Created by ccjeff on 2021/3/17.
//

#ifndef SCANNER_REGEXTREE_H
#define SCANNER_REGEXTREE_H

#include <memory>
#include <string_view>
#include <unordered_set>
#include <vector>
namespace {
    static const std::unordered_set<std::size_t> emptySet;
    /* used to pass the set outside of the FollowPos function
    otherwise since this is created locally, it will
    be out of scope once function returns. */
}

class regexTree {
public:
    // construction of the regex tree began with a cat node as the root
    explicit regexTree(std::string_view regex):root(catEndNode(buildTree(regex))), alphabet(Alphabet(root.get())){
        calcFollowPos(root.get());
    }

    //returns the unordered set of unique chars in the regex
    const std::unordered_set<char>& Alphabet() const {return alphabet;}
    //returns the firstpos set for the root
    const std::unordered_set<std::size_t>& firstPosRoot() const {
        return root->firstpos;
    }
    const std::unordered_set<std::size_t>& followPos(std::size_t pos) const {
        if (pos < this->leaves.size()){
            return this->leaves[pos]->followpos;
        }
        return emptySet;
    }

    bool charAtPos(char character, std::size_t pos) const {
        if (pos < this->leaves.size()){
            return this->leaves[pos]->label == character;
        }
        return false;
    }

    // returns the position of the end of regex. Equals the number of leaves in regex tree
    std::size_t endPos() const {return this->leaves.size();}


private:
    class Node {
    public:
        virtual ~Node() = 0;
        std::unordered_set<std::size_t> firstpos;
        std::unordered_set<std::size_t> lastpos;
        bool nullable = false;
};
    class CatNode : public Node {
    public:
        // initializer in constructor
        explicit CatNode(std::unique_ptr<Node> lptr, std::unique_ptr<Node> rptr) : left(std::move(lptr)), right(std::move(rptr)) {
            firstpos.insert(left->firstpos.cbegin(), left->firstpos.cend());
            // if left is nullable, firstops is the union of all subset firstops.
            if (left->nullable) {
                firstpos.insert(right->firstpos.cbegin(), right->firstpos.cend());
            }
            lastpos.insert(right->lastpos.cbegin(),right->lastpos.cend());
            if (right->nullable){
                lastpos.insert(left->lastpos.cbegin(),left->lastpos.cend());
            }
            nullable = left->nullable && right->nullable;
        }

        std::unique_ptr<Node> left;
        std::unique_ptr<Node> right;
    };

    class UnionNode : public Node {
    public:
        explicit UnionNode(std::unique_ptr<Node>lptr, std::unique_ptr<Node>rptr) : left(std::move(lptr)), right(std::move(rptr)){
            firstpos.insert(left->firstpos.cbegin(), left->firstpos.cend());
            firstpos.insert(right->firstpos.cbegin(), right->firstpos.cend());

            lastpos.insert(left->firstpos.cbegin(), left->firstpos.cend());
            lastpos.insert(right->firstpos.cbegin(), right->firstpos.cend());

            nullable = left->nullable || right->nullable;
        }

        std::unique_ptr<Node> left;
        std::unique_ptr<Node> right;
    };

    class StarNode: public Node {
    public:
        explicit StarNode(std::unique_ptr<Node> c) : child(std::move(c)){
            firstpos = child->firstpos;
            lastpos = child->lastpos;
            nullable = true;
        }

        std::unique_ptr<Node> child;
    };

    class LeafNode : public Node{
    public:
        explicit LeafNode(std::size_t pos, char l) : label(l){
            firstpos.emplace(pos);
            lastpos.emplace(pos);
        }
        std::unordered_set<std::size_t> followpos;
        char label;
    };

    class EndNode : public Node{
    public:
        explicit EndNode(std::size_t end_pos){
            firstpos.insert(end_pos);
        }
    };

    std::unique_ptr<Node> buildTree(std::string_view regex, bool star = false);

    //create End Node and concat it with the root
    std::unique_ptr<Node> catEndNode(std::unique_ptr<regexTree::Node> root);
    std::unordered_set<char> Alphabet(Node* n);
    void calcFollowPos(Node* n);

    std::vector<LeafNode*> leaves;
    std::unique_ptr<Node> root;
    std::unordered_set<char> alphabet;

};


#endif //SCANNER_REGEXTREE_H
