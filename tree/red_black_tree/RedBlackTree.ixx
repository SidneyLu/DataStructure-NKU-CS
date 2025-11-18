//
// Created by 31515 on 25-11-18.
//

export module RedBlackTree;

import BinaryTreeNode;
import BinarySearchTreeNode;
import BinarySearchTree;

import std;

export template<typename T, typename F = NodeValueComparator<T>>
class RedBlackTree : public BinarySearchTree<T, F> {
public:
    RedBlackTree() : BinarySearchTree<T, F>() {}

    void insert(T value) {
        BinarySearchTree<T, F>::insert(value);
        BinarySearchTreeNode<T, F>* insertedNode = this->root->find(value);

        if (this->nodeComparator.equal(insertedNode->value, this->root->value)) {
            this->setBlack(insertedNode);
        } else {
            this->setRed(insertedNode);
        }

        this->balance(insertedNode);
    }

    bool remove(T value) override {

    }

    void balance(BinarySearchTreeNode<T, F>* node) {
        if (this->nodeComparator.equal(node->value, this->root->value)) {
            return;
        }

        if (this->isBlack(node->parent)) {
            return;
        }

        auto grandParent = node->parent->parent;

        if (node->uncle() && this->isRed(node->uncle()))
        {
            this->setBlack(node->uncle());
            this->setBlack(node->parent);

            if (!this->nodeComparator.equal(grandParent->value, this->root->value)) {
                this->setRed(grandParent);
            }
            else {
                return;
            }

            this->balance(grandParent);
        }
        else if (!node->uncle() || this->isBlack(node->uncle())) {
            if (grandParent) {
                BinarySearchTreeNode<T, F>* newGrandParent;

                if (this->nodeComparator.equal(grandParent->left->value, node->parent->value)) {
                    if (this->nodeComparator.equal(node->parent->left->value, node->value)) {
                        newGrandParent = this->rotateLeftLeft(grandParent);
                    }
                    else {
                        newGrandParent = this->rotateLeftRight(grandParent);
                    }
                }
                else {
                    if (this->nodeComparator.equal(node->parent->right->value, node->value)) {
                        newGrandParent = this->rotateRightRight(grandParent);
                    }
                    else {
                        newGrandParent = this->rotateRightLeft(grandParent);
                    }
                }

                if (newGrandParent && newGrandParent->parent = nullptr) {
                    this->root = newGrandParent;
                    this->setBlack(newGrandParent);
                }

                this->balance(newGrandParent);
            }
        }
    }

    BinarySearchTreeNode<T, F>* rotateLeftLeft(BinarySearchTreeNode<T, F>* grandParentNode) {
        BinarySearchTreeNode<T, F>* grandGrandParent = grandParentNode->parent;

        bool grandParentNodeIsLeft;
        if (grandGrandParent) {
            grandParentNodeIsLeft = this->nodeComparator.equal(grandGrandParent->left, grandParentNode);
        }

        BinarySearchTreeNode<T, F>* parentNode = grandParentNode->left;
        BinarySearchTreeNode<T, F>* parentRightNode = parentNode->right;

        parentNode->setRight(grandParentNode);
        grandParentNode->setLeft(parentRightNode);

        if (grandGrandParent) {
            if (grandParentNodeIsLeft) {
                grandGrandParent->setLeft(parentNode);
            }
            else {
                grandGrandParent->setRight(parentNode);
            }
        }
        else {
            parentNode->parent = nullptr;
        }

        this->swapNodeColors(parentNode, grandParentNode);

        return parentNode;
    }

    BinarySearchTreeNode<T, F>* rotateLeftRight(BinarySearchTreeNode<T, F>* grandParentNode) {
        BinarySearchTreeNode<T, F>* parentNode = grandParentNode->left;
        BinarySearchTreeNode<T, F>* childNode = parentNode->right;

        BinarySearchTreeNode<T, F>* childLeftNode = childNode->left;

        childNode->setLeft(parentNode);
        grandParentNode->setLeft(childNode);

        parentNode->setRight(childLeftNode);

        return this->rotateRightLeft(grandParentNode);
    }

    BinarySearchTreeNode<T, F>* rotateRightLeft(BinarySearchTreeNode<T, F>* grandParentNode) {
        BinarySearchTreeNode<T, F>* parentNode = grandParentNode->right;
        BinarySearchTreeNode<T, F>* childNode = parentNode->left;

        BinarySearchTreeNode<T, F>* childRightNode = childNode->right;

        childNode->setRight(parentNode);
        grandParentNode->setRight(childNode);

        parentNode->setLeft(childRightNode);

        return this->rotateLeftLeft(grandParentNode);
    }

    BinarySearchTreeNode<T, F>* rotateRightRight(BinarySearchTreeNode<T, F>* grandParentNode) {
        BinarySearchTreeNode<T, F>* grandGrandParent = grandParentNode->parent;

        bool grandParentNodeIsLeft;
        if (grandGrandParent) {
            grandParentNodeIsLeft = this->nodeComparator.equal(grandGrandParent->left, grandParentNode);
        }

        BinarySearchTreeNode<T, F>* parentNode = grandParentNode->right;
        BinarySearchTreeNode<T, F>* parentLeftNode = parentNode->left;

        parentNode->setLeft(grandParentNode);
        grandParentNode->setRight(parentLeftNode);

        if (grandGrandParent) {
            if (grandParentNodeIsLeft) {
                grandGrandParent->setLeft(parentNode);
            }
            else {
                grandGrandParent->setRight(parentNode);
            }
        }
        else {
            parentNode->parent = nullptr;
        }

        this->swapNodeColors(parentNode, grandParentNode);

        return parentNode;
    }

    BinarySearchTreeNode<T, F>* setBlack(BinarySearchTreeNode<T, F>* node) {
        node->color = Color::Black;
        return node;
    }

    BinarySearchTreeNode<T, F>* setRed(BinarySearchTreeNode<T, F>* node) {
        node->color = Color::Red;
        return node;
    }

    bool isBlack(const BinaryTreeNode<T, F>* node) const {
        return node->color == Color::Black;
    }

    bool isRed(const BinaryTreeNode<T, F>* node) const {
        return node->color == Color::Red;
    }

    void swapNodeColors(BinaryTreeNode<T, F>* node1, BinaryTreeNode<T, F>* node2) {
        Color c1 = node1->color;
        Color c2 = node2->color;
        node1->color = c2;
        node2->color = c1;
    }

    ~RedBlackTree() override {}
};