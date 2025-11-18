export module AvlTree;

import BinaryTreeNode;
import BinarySearchTreeNode;
import BinarySearchTree;

import std;

export template<typename T, typename F = NodeValueComparator<T>>
class AvlTree : public BinarySearchTree<T, F> {
public:
    AvlTree() : BinarySearchTree<T, F>() {}

    void insert(T value) {
        BinarySearchTree<T, F>::insert(value);
        BinarySearchTreeNode<T, F>* currentNode = this->root->find(value);
        while (currentNode) {
            this->balance(currentNode);
            currentNode = currentNode->parent;
        }
    }

    bool remove(T value) override {
        bool target = BinarySearchTree<T, F>::remove(value);
        this->balance(this->root);
        return target;
    }

    void balance(BinarySearchTreeNode<T, F>* node) {
        if (node->balanceFactor() > 1) {
            if (node->left->balanceFactor() > 0) {
                this->rotateLeftLeft(node);
            } else if (node->left->balanceFactor() < 0) {
                this->rotateLeftRight(node);
            }
        } else if (node->balanceFactor() < -1) {
            if (node->right->balanceFactor() < 0) {
                this->rotateRightRight(node);
            } else if (node->right->balanceFactor() > 0) {
                this->rotateRightLeft(node);
            }
        }
    }

    void rotateLeftLeft(BinarySearchTreeNode<T, F>* rootNode) {
        const BinarySearchTreeNode<T, F>* leftNode = rootNode->left;
        rootNode->setLeft(nullptr);

        if (rootNode->parent) {
            rootNode->parent->setLeft(leftNode);
        } else if (rootNode == this->root) {
            this->root = leftNode;
        }

        if (leftNode->right) {
            rootNode->setLeft(leftNode->right);
        }

        leftNode->setRight(rootNode);
    }

    void rotateLeftRight(BinarySearchTreeNode<T, F>* rootNode) {
        const BinarySearchTreeNode<T, F>* leftNode = rootNode->left;
        rootNode->setLeft(nullptr);

        const BinarySearchTreeNode<T, F>* leftRightNode = leftNode->right;
        leftNode->setRight(nullptr);

        if (leftRightNode->left) {
            leftNode->setRight(leftRightNode->left);
            leftRightNode->setLeft(nullptr);
        }

        rootNode->setLeft(leftRightNode);
        leftRightNode->setLeft(leftNode);

        this->rotateLeftLeft(rootNode);
    }

    void rotateRightLeft(BinarySearchTreeNode<T, F>* rootNode) {
        const BinarySearchTreeNode<T, F>* rightNode = rootNode->right;
        rootNode->setRight(nullptr);

        const BinarySearchTreeNode<T, F>* rightLeftNode = rightNode->left;
        rightNode->setLeft(nullptr);

        if (rightLeftNode->right) {
            rightNode->setRight(rightLeftNode->right);
            rightLeftNode->setRight(nullptr);
        }

        rootNode->setRight(rightLeftNode);
        rightLeftNode->setRight(rightNode);

        this->rotateRightRight(rootNode);
    }

    void rotateRightRight(BinarySearchTreeNode<T, F>* rootNode) {
        BinarySearchTreeNode<T, F>* rightNode = rootNode->right;
        rootNode->setRight(nullptr);

        if (rootNode->parent) {
            rootNode->parent->setRight(rightNode);
        } else if (rootNode == this->root) {
            this->root = rightNode;
        }

        if (rightNode->left) {
            rootNode->setRight(rightNode->left);
        }

        rightNode->left->setRight(rootNode);
    }

    ~AvlTree() override {
        // 父类BinarySearchTree的虚析构函数会自动处理根节点的内存释放
        // 这里不需要额外操作，因为多态会确保正确的析构顺序
    }

};