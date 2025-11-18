//
// Created by 31515 on 25-11-15.
//

export module BinarySearchTree;

import BinaryTreeNode;
import BinarySearchTreeNode;

import std;

export template<typename T, typename F = NodeValueComparator<T>>
class BinarySearchTree {
protected:
    BinarySearchTreeNode<T, F>* root;
    F nodeComparator;

public:
    explicit BinarySearchTree(const F& nodeComparator = F{}) : nodeComparator(nodeComparator) {
        this->root = new BinarySearchTreeNode<T, F>();
    }

    BinarySearchTree& insert(T value) {
        return this->root->insert(value);
    }

    bool contains(T value) {
        return this->root->contains(value);
    }

    virtual bool remove(T value) {
        return this->root->remove(value);
    }

    std::string toString() {
        return this->root->toString();
    }

    virtual ~BinarySearchTree() {
        // 删除根节点，触发整个树的递归内存释放
        if (root != nullptr) {
            delete root;
            root = nullptr;
        }
    }
};