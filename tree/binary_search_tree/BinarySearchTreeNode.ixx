//
// Created by 31515 on 25-11-15.
//

export module BinarySearchTreeNode;

import BinaryTreeNode;

import std;

export template<typename T, typename F = NodeValueComparator<T>>
class BinarySearchTreeNode : public BinaryTreeNode<T, F>
{
public:
    explicit BinarySearchTreeNode(T value = 0, const F& nodeComparator = F{})
        : BinaryTreeNode<T, F>(value, nodeComparator) {}

    BinarySearchTreeNode(const BinarySearchTreeNode& other)
       : BinaryTreeNode<T, F>(other.value, other.nodeComparator)
    {
        if (other.left != nullptr) {
            this->left = new BinarySearchTreeNode(*other.left);
            this->left->parent = this;
        }

        if (other.right != nullptr) {
            this->right = new BinarySearchTreeNode(*other.right);
            this->right->parent = this;
        }
    }

    BinarySearchTreeNode& insert(const T& value) {
        if (!this->value) {
            this->value = value;
            return *this;
        }

        if (this->nodeComparator.lessThan(value, this->value)) {
            if (this->left) {
                return this->left->insert(value);
            }

            auto newNode = new BinarySearchTreeNode(value, this->nodeComparator);
            this->setLeft(newNode);

            return *newNode;
        }

        if (this->nodeComparator.greaterThan(value, this->value)) {
            if (this->right) {
                return this->right->insert(value);
            }

            auto newNode = new BinarySearchTreeNode(value, this->nodeComparator);
            this->setRight(newNode);

            return *newNode;
        }

        return *this;
    }

    BinarySearchTreeNode* find(T value) {
        if (this->nodeComparator.equal(this->value, value)) {
            return this;
        }

        if (this->nodeComparator.lessThan(value, this->value) && this->left) {
            return this->left->find(value);
        }

        if (this->nodeComparator.greaterThan(value, this->value) && this->right) {
            return this->right->find(value);
        }

        return nullptr;
    }

    bool contains(T value) {
        return this->find(value) != nullptr;
    }

    bool remove(const T& value) {
        BinarySearchTreeNode* nodeToRemove = this->find(value);

        BinarySearchTreeNode* parent = static_cast<BinarySearchTreeNode*>(nodeToRemove->parent);

        if (!nodeToRemove->left && !nodeToRemove->right) {
            if (parent) {
                if (parent->left == nodeToRemove) {
                    delete parent->left;
                    parent->left = nullptr;
                } else {
                    delete parent->right;
                    parent->right = nullptr;
                }
            } else {
                delete nodeToRemove;
            }
        }

        else if (nodeToRemove->left && nodeToRemove->right) {
            BinarySearchTreeNode* rightChild = static_cast<BinarySearchTreeNode*>(nodeToRemove->right);
            BinarySearchTreeNode* nextBiggerNode = rightChild->findMin();

            if (!(this->nodeComparator.equal(nextBiggerNode->value, rightChild->value))) {
                this->remove(nextBiggerNode->value);
                nodeToRemove->value = nextBiggerNode->value;
            } else {
                // 后继是右子节点：直接替换值，调整右子树（直接访问 right）
                nodeToRemove->value = rightChild->value;
                nodeToRemove->right = rightChild->right; // 右子树指向后继的右子树
                // 若新右子树存在，更新其父指针
                if (nodeToRemove->right) {
                    nodeToRemove->right->parent = nodeToRemove;
                }
                // 释放原右子节点（后继节点）
                delete rightChild;
            }
        }
        // 5. 情况3：只有一个子节点（直接访问 left/right）
        else {
            // 获取唯一子节点（左或右，直接访问）
            BinarySearchTreeNode* childNode =
                (nodeToRemove->left != nullptr)
                ? static_cast<BinarySearchTreeNode*>(nodeToRemove->left)
                : static_cast<BinarySearchTreeNode*>(nodeToRemove->right);

            if (parent) {
                // 父节点替换当前节点为子节点（直接修改父节点的 left/right）
                if (parent->left == nodeToRemove) {
                    parent->left = childNode; // 父节点左子指针指向子节点
                } else {
                    parent->right = childNode; // 父节点右子指针指向子节点
                }
                childNode->parent = parent; // 更新子节点的父指针
                delete nodeToRemove; // 释放当前节点内存
            } else {
                // 根节点：用子节点内容覆盖当前节点，再释放子节点
                nodeToRemove->value = childNode->value;       // 直接修改 value
                nodeToRemove->left = childNode->left;         // 直接修改 left
                nodeToRemove->right = childNode->right;       // 直接修改 right

                if (childNode->left) {
                    childNode->left->parent = nodeToRemove;
                }
                if (childNode->right) {
                    childNode->right->parent = nodeToRemove;
                }

                childNode->left = nullptr;
                childNode->right = nullptr;
                childNode->parent = nullptr;
                delete childNode;
            }
        }

        return true;
    }
    
     BinarySearchTreeNode* findMin() {
        if (!this->left) {
            return this;
        }
        return this->left->findMin();
    }

    ~BinarySearchTreeNode() override {
        // 父类BinaryTreeNode的虚析构函数会自动处理左右子节点的内存释放
        // 这里不需要额外操作，因为多态会确保正确的析构顺序
    }
};